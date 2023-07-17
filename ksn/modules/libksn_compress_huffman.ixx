
export module libksn.compress:huffman;

import <ksn/ksn.hpp>;
import :common;

#ifdef _KSN_COMPILER_MSVC
#pragma warning(disable : 4804)
#endif



_KSN_EXPORT_BEGIN

template<class in_t = uint8_t, class ByteAlloc = std::allocator<uint8_t>>
class huffman_compressor
{
	using out_t = uint8_t;
	std::vector<out_t, ByteAlloc> buff;

	using my_t = huffman_compressor<in_t, ByteAlloc>;

public:
	using in_type = in_t;
	using out_type = out_t;

	void compress(in_span_t<my_t>);
	void finish();

	out_span_t<my_t> data();
	void flush();
	void reset();
};

_KSN_EXPORT_END





_KSN_BEGIN

size_t align_up_8(size_t x)
{
	return (x + 7) & ~7;
}

uint8_t ror8(uint8_t x)
{
	return (x >> 1) | (x << 7);
}
uint8_t rol8(uint8_t x)
{
	return (x << 1) | (x >> 7);
}
template<class T, class U>
T safe_cast(U&& val)
{
	assert(val == T(val));
	return T(std::forward<U>(val));
}


struct bit_iterator
{
	uint8_t* ptr;
	uint8_t mask = 1;
	uint16_t bit = 0;

	bit_iterator(uint8_t* p) : ptr(p) {}
	bit_iterator(uint8_t* p, uint16_t bit)
	{
		this->bit = bit;
		ptr = p + bit / 8;
		mask = 1 << (bit % 8);
	}

	bool read() const
	{
		return *ptr & mask;
	}
	void write(bool x) const
	{
		auto& val = *ptr;
		val = (val & ~mask) | (-x & mask);
	}
	bit_iterator& operator++()
	{
		mask = rol8(mask);
		ptr += (mask == 1);
		++bit;
		return *this;
	}
	bit_iterator& operator--()
	{
		mask = ror8(mask);
		ptr -= (mask == 128);
		--bit;
		return *this;
	}
	bit_iterator operator++(int)
	{
		auto copy = *this;
		++*this;
		return copy;
	}
	bit_iterator operator--(int)
	{
		auto copy = *this;
		--*this;
		return copy;
	}
};

bit_iterator bit_iterator_copy(bit_iterator dst, bit_iterator src, size_t n)
{
	while (n-- > 0)
	{
		dst.write(src.read());
		++dst; ++src;
	}
	return dst;
};

template<class T>
bit_iterator bit_stream_append_obj(bit_iterator p, const T& x, size_t bits = -1)
{
	bits = std::min(bits, sizeof(T) * 8);
	bit_iterator src((uint8_t*)&x);
	return bit_iterator_copy(p, src, bits);
}


struct leaf_node_data
{
	uint8_t byte;
};
struct helper_node_data
{
	uint16_t left, right;
};

struct tree_node
{
	uint32_t count;
	std::variant<leaf_node_data, helper_node_data> data;

	void set_children(uint16_t a, uint16_t b)
	{
		data = helper_node_data{ a, b };
	}
	void set_byte(uint8_t val, uint32_t count)
	{
		this->count = count;
		this->data = leaf_node_data{ val };
	}
	bool has_byte() const
	{
		return std::holds_alternative<leaf_node_data>(this->data);
	}
	uint8_t get_byte() const
	{
		return std::get<leaf_node_data>(this->data).byte;
	}
};



template<class in_t, class ByteAlloc>
void huffman_compressor<in_t, ByteAlloc>::compress(in_span_t<my_t> _span)
{
	const uint8_t* p = (uint8_t*)_span.data(), *const pe = (uint8_t*)(_span.data() + _span.size());

	uint32_t bytes_entry_counts[256]{};
	while (p != pe)
		++bytes_entry_counts[*p++];

	tree_node nodes[512];
	size_t nodes_count = 0;
	uint16_t root_node = 0;





	uint16_t heap[256];
	size_t heap_size = 0;
	std::span<uint16_t> heap_span;

	auto update_heap_span = [&]
	{
		heap_span = std::span(heap, heap + heap_size);
	};

	auto node_id = [&]
	(tree_node& node)
	{
		return uint16_t(&node - &nodes[0]);
	};

	auto heap_comparator = [&]
	(uint16_t a, uint16_t b)
	{
		return nodes[a].count > nodes[b].count;
	};

	auto heap_append = [&]
	(tree_node& node)
	{
		uint16_t id = node_id(node);
		heap[heap_size++] = id;
	};

	auto heap_push = [&]
	(tree_node& node)
	{
		heap_append(node);
		update_heap_span();
		std::ranges::push_heap(heap_span, heap_comparator);
	};

	auto heap_pop = [&]() -> auto&
	{
		std::ranges::pop_heap(heap_span, heap_comparator);
		uint16_t id = heap[--heap_size];
		update_heap_span();
		return nodes[id];
	};

	auto buy_node = [&]() -> auto&
	{
		return nodes[nodes_count++];
	};

	size_t storage_size = 114;
	for (size_t byte = 0; byte < 256; ++byte)
	{
		auto count = bytes_entry_counts[byte];
		if (count == 0)
			continue;

		auto& node = buy_node();
		node.set_byte((uint8_t)byte, count);
		heap_append(node);
		storage_size += 9;
	}

	update_heap_span();
	std::ranges::make_heap(heap_span, heap_comparator);


	storage_size += 19 * (heap_size - 1);
	while (heap_size > 1)
	{
		auto& n1 = heap_pop();
		auto& n2 = heap_pop();
		auto& n3 = buy_node();
		n3.count = n1.count + n2.count;
		n3.set_children(node_id(n1), node_id(n2));
		heap_push(n3);
		root_node = node_id(n3);
	}





	struct byte_mapping_data
	{
		uint16_t offset;
		uint8_t length;
	} mappings[256];

	auto byte_mapping_offset = [&]
	(byte_mapping_data& mapping) -> auto&
	{
		return mapping.offset;
	};
	auto byte_mapping_length = [&]
	(byte_mapping_data& mapping) -> auto&
	{
		return mapping.length;
	};


	size_t data_storage_size = 0;
	for (int x = 0; x < 256; ++x)
	{
		auto count = bytes_entry_counts[x];
		data_storage_size += mappings[x].length * count;
	}
	storage_size += data_storage_size;



	this->buff.resize((storage_size + 7) / 8);
	bit_iterator out(this->buff.data());

	out = bit_stream_append_obj(out, (uint32_t)0x15269E85);
	out = bit_stream_append_obj(out, (uint64_t)data_storage_size);
	out = bit_stream_append_obj(out, nodes_count, 9);
	out = bit_stream_append_obj(out, root_node, 9);


	uint8_t bitseq_storage[4112];
	uint8_t bitseq_temp[32];

	bit_iterator bitseq_storage_iterator(bitseq_storage);
	bit_iterator bitseq_temp_iterator(bitseq_temp), bitseq_temp_begin = bitseq_temp_iterator;


	//TODO: put metadata
	/*
	metadata section:
		sig?: 32 bits: compression signature "KSNHUF", 5 bit per letter
		n_data: 64? bits: data section bits
		n_nodes: 9 bits: stored tree nodes count
		root: 9 bits: tree root index
		tree: {
			leaf: 1 bit: is node leaf?
			[leaf=1] byte: 8 bits: leaf byte value
			[leaf=0] children: 9 bits[2]: node children
		}[n_nodes]

	data section:
		data: 1 bit[n_data]: compressed data
	*/

	auto traverse_node_tree = [&]
	(this auto&& recursion, uint16_t node_index) -> void
	{
		const auto& node = nodes[node_index];
		const bool has_byte = node.has_byte();
		//out = 
		if (has_byte)
		{
			auto& mapping = mappings[node.get_byte()];

			byte_mapping_length(mapping) = safe_cast<uint8_t>(bitseq_temp_iterator.bit);
			byte_mapping_offset(mapping) = bitseq_storage_iterator.bit;
			bitseq_storage_iterator = bit_iterator_copy(bitseq_storage_iterator, bitseq_temp_begin, mapping.length);
		}
		else
		{
			const auto [left, right] = std::get<helper_node_data>(node.data);

			bitseq_temp_iterator++.write(0);
			recursion(left);
			--bitseq_temp_iterator;

			bitseq_temp_iterator++.write(1);
			recursion(right);
			--bitseq_temp_iterator;
		}
	};

	traverse_node_tree(root_node);


	for (p = (uint8_t*)_span.data(); p != pe; ++p)
	{
		const auto byte = *p;
		auto count = bytes_entry_counts[byte];
		const auto& mapping = mappings[byte];

		bit_iterator bp(bitseq_storage, mapping.offset);
		out = bit_iterator_copy(out, bp, mapping.length);
	}
	__debugbreak();
}

template<class in_t, class ByteAlloc>
void huffman_compressor<in_t, ByteAlloc>::finish()
{
}

template<class in_t, class ByteAlloc>
auto huffman_compressor<in_t, ByteAlloc>::data() -> out_span_t<my_t>
{
	return this->buff;
}

template<class in_t, class ByteAlloc>
void huffman_compressor<in_t, ByteAlloc>::flush()
{
	this->buff.clear();
}

template<class in_t, class ByteAlloc>
void huffman_compressor<in_t, ByteAlloc>::reset()
{
	this->flush();
}

_KSN_END
