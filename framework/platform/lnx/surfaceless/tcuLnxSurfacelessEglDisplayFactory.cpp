/*-------------------------------------------------------------------------
 * drawElements Quality Program Tester Core
 * ----------------------------------------
 *
 * Copyright 2014 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *//*!
 * \file
 * \brief Surfaceless Egl Display Factory.
 *//*--------------------------------------------------------------------*/

#include "tcuLnxSurfacelessEglDisplayFactory.hpp"
//XXX#include "tcuLnxSurfaceless.hpp"
#include "egluGLContextFactory.hpp"
#include "eglwLibrary.hpp"
#include "eglwFunctions.hpp"
#include "eglwEnums.hpp"
#include "deUniquePtr.hpp"

#ifndef EGL_PLATFORM_SURFACELESS_MESA
#define EGL_PLATFORM_SURFACELESS_MESA     0x31DD
#endif

namespace tcu
{
namespace lnx
{
namespace surfaceless
{
namespace egl
{

#if 0
typedef ::Display*	EGLNativeDisplayType;
typedef ::Pixmap	EGLNativePixmapType;
typedef ::Window	EGLNativeWindowType;

DE_STATIC_ASSERT(sizeof(EGLNativeDisplayType)	<= sizeof(eglw::EGLNativeDisplayType));
DE_STATIC_ASSERT(sizeof(EGLNativePixmapType)	<= sizeof(eglw::EGLNativePixmapType));
DE_STATIC_ASSERT(sizeof(EGLNativeWindowType)	<= sizeof(eglw::EGLNativeWindowType));

extern "C"
{

typedef EGLW_APICALL	eglw::EGLDisplay	(EGLW_APIENTRY* eglSurfacelessGetDisplayFunc)			(EGLNativeDisplayType display_id);
typedef EGLW_APICALL	eglw::EGLBoolean	(EGLW_APIENTRY* eglSurfacelessCopyBuffersFunc)			(eglw::EGLDisplay dpy, eglw::EGLSurface surface, EGLNativePixmapType target);
typedef EGLW_APICALL	eglw::EGLSurface	(EGLW_APIENTRY* eglSurfacelessCreatePixmapSurfaceFunc)	(eglw::EGLDisplay dpy, eglw::EGLConfig config, EGLNativePixmapType pixmap, const eglw::EGLint* attrib_list);
typedef EGLW_APICALL	eglw::EGLSurface	(EGLW_APIENTRY* eglSurfacelessCreateWindowSurfaceFunc)	(eglw::EGLDisplay dpy, eglw::EGLConfig config, EGLNativeWindowType win, const eglw::EGLint* attrib_list);

}
#endif

using std::string;

using de::MovePtr;
using de::UniquePtr;
using glu::ContextFactory;
using eglu::GLContextFactory;
using eglu::NativeDisplay;
using eglu::NativeDisplayFactory;
using eglu::NativeWindow;
using eglu::NativeWindowFactory;
using eglu::NativePixmap;
using eglu::NativePixmapFactory;
using eglu::WindowParams;
using tcu::TextureLevel;

class Library : public eglw::DefaultLibrary
{
public:
	Library (void)
		: eglw::DefaultLibrary("libEGL.so")
	{
	}
#if 0
	eglw::EGLBoolean copyBuffers (eglw::EGLDisplay dpy, eglw::EGLSurface surface, eglw::EGLNativePixmapType target) const
	{
		return ((eglSurfacelessCopyBuffersFunc)m_egl.copyBuffers)(dpy, surface, reinterpret_cast<EGLNativePixmapType>(target));
	}

	eglw::EGLSurface createPixmapSurface (eglw::EGLDisplay dpy, eglw::EGLConfig config, eglw::EGLNativePixmapType pixmap, const eglw::EGLint *attrib_list) const
	{
		return ((eglSurfacelessCreatePixmapSurfaceFunc)m_egl.createPixmapSurface)(dpy, config, reinterpret_cast<EGLNativePixmapType>(pixmap), attrib_list);
	}

	eglw::EGLSurface createWindowSurface (eglw::EGLDisplay dpy, eglw::EGLConfig config, eglw::EGLNativeWindowType win, const eglw::EGLint *attrib_list) const
	{
		return ((eglSurfacelessCreateWindowSurfaceFunc)m_egl.createWindowSurface)(dpy, config, reinterpret_cast<EGLNativeWindowType>(win), attrib_list);
	}

	eglw::EGLDisplay getDisplay (eglw::EGLNativeDisplayType display_id) const
	{
		return ((eglSurfacelessGetDisplayFunc)m_egl.getDisplay)(reinterpret_cast<EGLNativeDisplayType>(display_id));
	}
#endif
};

class Display : public NativeDisplay
{
public:
	static const Capability CAPABILITIES		= Capability(CAPABILITY_GET_DISPLAY_PLATFORM);

								Display				(void)
									: NativeDisplay	(CAPABILITIES,
													 EGL_PLATFORM_SURFACELESS_MESA,
													 "EGL_MESA_platform_surfaceless") {}

	void*						getPlatformNative		(void)	{ return DE_NULL; }
	//XXX eglw::EGLNativeDisplayType	getLegacyNative			(void)	{ return reinterpret_cast<eglw::EGLNativeDisplayType>(m_display->getXDisplay()); }

	//XXX XlibDisplay&				getSurfacelessDisplay			(void)			{ return *m_display;	}
	const eglw::Library&		getLibrary				(void) const	{ return m_library;		}
	const eglw::EGLAttrib*		getPlatformAttributes	(void) const	{ return DE_NULL;		}

private:
	//XXX UniquePtr<XlibDisplay>		m_display;
	Library						m_library;
};

#if 0
class Window : public NativeWindow
{
public:
	static const Capability	CAPABILITIES		= Capability(CAPABILITY_CREATE_SURFACE_LEGACY |
															 CAPABILITY_CREATE_SURFACE_PLATFORM |
															 CAPABILITY_GET_SURFACE_SIZE |
															 CAPABILITY_SET_SURFACE_SIZE |
															 CAPABILITY_GET_SCREEN_SIZE);

								Window				(Display&				display,
													 const WindowParams&	params,
													 Visual*				visual);

	eglw::EGLNativeWindowType	getLegacyNative		(void) { return reinterpret_cast<eglw::EGLNativeWindowType>(m_window.getXID()); }
	void*						getPlatformNative	(void) { return &m_window.getXID();	}

	IVec2						getSurfaceSize		(void) const;
	void						setSurfaceSize		(IVec2 size);
	IVec2						getScreenSize		(void) const { return getSurfaceSize(); }

private:
	XlibWindow					m_window;
};

Window::Window (Display& display, const WindowParams& params, Visual* visual)
	: NativeWindow	(CAPABILITIES)
	, m_window		(display.getSurfacelessDisplay(), params.width, params.height, visual)
{
	m_window.setVisibility((params.visibility != WindowParams::VISIBILITY_HIDDEN));
}

IVec2 Window::getSurfaceSize (void) const
{
	IVec2 ret;
	m_window.getDimensions(&ret.x(), &ret.y());
	return ret;
}

void Window::setSurfaceSize (IVec2 size)
{
	m_window.setDimensions(size.x(), size.y());
}

class WindowFactory : public NativeWindowFactory
{
public:
						WindowFactory		(void);

	NativeWindow*		createWindow		(NativeDisplay*			nativeDisplay,
											 const WindowParams&	params) const;

	NativeWindow*		createWindow		(NativeDisplay*			nativeDisplay,
											 eglw::EGLDisplay		display,
											 eglw::EGLConfig		config,
											 const eglw::EGLAttrib*	attribList,
											 const WindowParams&	params) const;
};

WindowFactory::WindowFactory (void)
	: NativeWindowFactory ("window", "Surfaceless Window", Window::CAPABILITIES)
{
}

NativeWindow* WindowFactory::createWindow (NativeDisplay*		nativeDisplay,
										   const WindowParams&	params) const
{
	Display&	display	= *dynamic_cast<Display*>(nativeDisplay);

	return new Window(display, params, DE_NULL);
}

NativeWindow* WindowFactory::createWindow (NativeDisplay*			nativeDisplay,
										   eglw::EGLDisplay			eglDisplay,
										   eglw::EGLConfig			config,
										   const eglw::EGLAttrib*	attribList,
										   const WindowParams&		params) const
{
	DE_UNREF(attribList);

	Display&		display		= *dynamic_cast<Display*>(nativeDisplay);
	eglw::EGLint	visualID	= 0;
	::Visual*		visual		= DE_NULL;
	nativeDisplay->getLibrary().getConfigAttrib(eglDisplay, config, EGL_NATIVE_VISUAL_ID, &visualID);

	if (visualID != 0)
		visual = display.getSurfacelessDisplay().getVisual(visualID);

	return new Window(display, params, visual);
}
#endif

#if 0
class Pixmap : public NativePixmap
{
public:
	enum {
		CAPABILITIES = (CAPABILITY_CREATE_SURFACE_LEGACY |
						CAPABILITY_CREATE_SURFACE_PLATFORM |
						CAPABILITY_READ_PIXELS)
	};

							Pixmap				(MovePtr<surfaceless::Pixmap> surfacelessPixmap)
								: NativePixmap	(CAPABILITIES)
								, m_pixmap		(surfacelessPixmap) {}

	void*					getPlatformNative	(void) { return &m_pixmap.getXID(); }
	void					readPixels			(TextureLevel* dst);

private:
	UniquePtr<surfaceless::Pixmap>	m_pixmap;
};

class PixmapFactory : public NativePixmapFactory
{
public:
					PixmapFactory	(void)
						: NativePixmapFactory ("pixmap", "Surfaceless Pixmap", Pixmap::CAPABILITIES) {}

	NativePixmap*	createPixmap	(NativeDisplay* nativeDisplay,
									 int			width,
									 int			height) const;
};

NativePixmap* PixmapFactory::createPixmap (NativeDisplay* nativeDisplay,
										   int			width,
										   int			height) const

{
	Display*				display		= dynamic_cast<Display*>(nativeDisplay);
	MovePtr<surfaceless::Pixmap>	surfacelessPixmap	(new surfaceless::Pixmap(display->getSurfacelessDisplay(),
														 width, height));
	return new Pixmap(surfacelessPixmap);
}
#endif

class DisplayFactory : public NativeDisplayFactory
{
public:
						DisplayFactory		(EventState& eventState);

	NativeDisplay*		createDisplay		(const eglw::EGLAttrib* attribList) const;

private:
	EventState&			m_eventState;
};

DisplayFactory::DisplayFactory (EventState& eventState)
	: NativeDisplayFactory	("surfaceless", "Surfaceless Display",
							 Display::CAPABILITIES,
							 EGL_PLATFORM_SURFACELESS_MESA,
							 "EGL_MESA_platform_surfaceless")
	, m_eventState			(eventState)
{
	//XXX m_nativeWindowRegistry.registerFactory(new WindowFactory());
	//XXX m_nativePixmapRegistry.registerFactory(new PixmapFactory());
}

NativeDisplay* DisplayFactory::createDisplay (const eglw::EGLAttrib* attribList) const
{
	DE_UNREF(attribList);

	return new Display();
}

NativeDisplayFactory* createDisplayFactory (EventState& eventState)
{
	return new DisplayFactory(eventState);
}

} // egl
} // surfaceless
} // lnx
} // tcu
