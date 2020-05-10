project('libqalculate', 'c', 'cpp', version: '3.10.0')

gnome = import('gnome')

root_dir = include_directories('.')

config_data = configuration_data()
config_data.set_quoted('VERSION', meson.project_version())
config_data.set_quoted('GETTEXT_PACKAGE', meson.project_name())
config_data.set_quoted('PACKAGE_DATA_DIR', get_option('prefix') / get_option('datadir'))
config_data.set_quoted('PACKAGE_LOCALE_DIR', get_option('prefix') / get_option('datadir') / 'locale')
config_data.set('ICONV_CONST', '')

if (get_option('compile_definitions').enabled())
  gio_dep = dependency('gio-2.0')
  subdir('data')
  config_data.set10('COMPILED_DEFINITIONS', true)
else
  gio_dep = []
  definition_resource = []
endif

if (get_option('libcurl').enabled())
  libcurl_dep = dependency('libcurl')
  config_data.set10('HAVE_LIBCURL', true)
else
  libcurl_dep = []
endif

if (get_option('icu').enabled())
  icu_uc_dep = dependency('icu-uc')
  config_data.set10('HAVE_ICU', true)
else
  icu_uc_dep = []
endif

cpp = meson.get_compiler('cpp')

mpfr_dep = cpp.find_library('mpfr')
gmp_dep = cpp.find_library('gmp')
#mpfr_dep = dependency('mpfr')
xml2_dep = dependency('libxml-2.0', version: '>=2.3.8')
thread_dep = dependency('threads')

config_file = configure_file(
  configuration: config_data,
  output: 'config.h'
)

add_project_arguments ('-DHAVE_CONFIG_H', language: 'cpp')

subdir('libqalculate')
