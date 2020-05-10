libqalculate_sources = files(
  'Function.cc',
  'Calculator.cc',
  'Calculator-plot.cc',
  'Calculator-parse.cc',
  'Calculator-definitions.cc',
  'Calculator-calculate.cc',
  'Calculator-convert.cc',
  'DataSet.cc',
  'Variable.cc',
  'ExpressionItem.cc',
  'Number.cc',
  'MathStructure.cc',
  'Prefix.cc',
  'support.h',
  'util.cc',
  'Unit.cc',
  'QalculateDateTime.cc',
  'BuiltinFunctions-util.cc',
  'BuiltinFunctions-trigonometry.cc',
  'BuiltinFunctions-matrixvector.cc',
  'BuiltinFunctions-datetime.cc',
  'BuiltinFunctions-statistics.cc',
  'BuiltinFunctions-explog.cc',
  'BuiltinFunctions-special.cc',
  'BuiltinFunctions-algebra.cc',
  'BuiltinFunctions-calculus.cc',
  'BuiltinFunctions-number.cc',
  'BuiltinFunctions-combinatorics.cc',
  'BuiltinFunctions-logical.cc',
  'MathStructure-factor.cc',
  'MathStructure-gcd.cc',
  'MathStructure-integrate.cc',
  'MathStructure-isolatex.cc',
  'MathStructure-polynomial.cc',
  'MathStructure-matrixvector.cc',
  'MathStructure-print.cc',
  'MathStructure-differentiate.cc',
  'MathStructure-calculate.cc',
  'MathStructure-eval.cc',
  'MathStructure-convert.cc',
  'MathStructure-decompose.cc',
  'MathStructure-limit.cc'
)

libqalculate_headers = files(
  'Function.h',
  'Calculator.h',
  'DataSet.h',
  'Variable.h',
  'ExpressionItem.h',
  'Number.h',
  'MathStructure.h',
  'Prefix.h',
  'util.h',
  'includes.h',
  'Unit.h',
  'BuiltinFunctions.h',
  'QalculateDateTime.h',
  'qalculate.h'
)

libqalculate_private_headers = files(
  'MathStructure-support.h',
  'Calculator_p.h',
  'bernoulli_numbers.h'
)

libqalculate_deps = [
    mpfr_dep,
    xml2_dep,
    gio_dep,
    libcurl_dep,
    icu_uc_dep,
    thread_dep
]

support_file = configure_file(
  configuration: config_data,
  output: 'support.h'
)

libqalculate_lib = library(
  'qalculate',
  libqalculate_sources,
  libqalculate_headers,
  libqalculate_private_headers,
  definition_resource,
  config_file,
  support_file,
  dependencies: libqalculate_deps,
  include_directories: root_dir,
  version : meson.project_version(),
  soversion : '3'
)

declare_dependency (
  link_with: 'libqalculate_lib',
  sources: definition_resource
)