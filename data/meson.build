xml_files = [
  'currencies.xml.in',
  'datasets.xml.in',
  'elements.xml.in',
  'functions.xml.in',
  'planets.xml.in',
  'prefixes.xml.in',
  'units.xml.in',
  'variables.xml.in'
]

xml_data = []
foreach xml_file : xml_files
  xml_data = custom_target(
    'intltool-merge-'xml_file,
    command: [
      'intltool-merge',
      '--xml-style',
      meson.current_source_dir() / '..' / 'po',
      '@INPUT@',
      '@OUTPUT@'
    ],
    input: xml_file,
    output: '@BASENAME@'
  )
endforeach

definition_resource = gnome.compile_resources(
  'definitions',
  'definitions.gresource.xml',
  source_dir: meson.current_build_dir(),
  dependencies: xml_data
)