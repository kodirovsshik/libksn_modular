
import libksn.window;

int main()
{
	ksn::graphics_api_opengl_settings ogl;
	ogl.ogl_version_major = 4;
	ogl.core_profile = true;

	ksn::window_t win(ogl);
}
