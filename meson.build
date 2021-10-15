project('comes-desktop-shell', 'cpp',
	version: '1.0.0',
	default_options: [
		'cpp_std=c++2a',
		'build.cpp_std=c++2a'
	])

deps = [
	dependency('gtk-layer-shell-0', version: '>= 0.6', fallback: ['gtk-layer-shell', 'gtk_layer_shell']),
	dependency('gtk+-3.0'),
	dependency('webkit2gtk-4.0'),
	dependency('vte-2.91')
]

sources = [
	'src/main.cc',
]

executable(
	'comes-desktop-shell',
	sources,
	dependencies: deps,
	install: true,
	include_directories: include_directories('')
)