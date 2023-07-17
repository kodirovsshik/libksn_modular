
import std;
import libksn.compress;



int main()
{
	ksn::huffman_compressor<char> c;
	c.compress("A DEAD DAD CEDED A BAD BABE A BEADED ABACA BED");
	auto data = c.data();
}
