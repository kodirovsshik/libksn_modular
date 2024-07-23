
import libksn.window;

int main()
{
	ksn::graphics_api_opengl_settings ogl;
	ogl.ogl_version_major = 4;
	ogl.core_profile = true;

	ksn::window win(ogl);

	auto result = win.open(800, 600);
	
	if (result == ksn::window_api_error::unimplemented)
		return 1;
	else if (!result)
		return -1;
}
