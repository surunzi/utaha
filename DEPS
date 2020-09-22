use_relative_paths = True

gclient_gn_args_file = 'build/config/gclient_args.gni'
gclient_gn_args = [
  'checkout_google_benchmark',
]

vars = {
  'chromium_git': 'https://chromium.googlesource.com',
  'build_revision': '153ad0bf09eda458f1ef4f74dcac5c12d530d770',
  'buildtools_revision': '204a35a2a64f7179f8b76d7a0385653690839e21',
  'trace_event_revision': '23ef5333a357fc7314630ef88b44c3a545881dee',
  'libc++_revision': 'd9040c75cfea5928c804ab7c235fed06a63f743a',
  'libc++abi_revision': '196ba1aaa8ac285d94f4ea8d9836390a45360533',
  'clang_revision': '92b362238013c401926b8a45b0b8f0a42d506120',
  'gn_version': 'git_revision:e002e68a48d1c82648eadde2f6aafa20d08c36f2',
  'v8_revision': '80dc1e8055bca07e8da6162bace5ee87f23faccf',
  'icu_revision': '79326efe26e5440f530963704c3c0ff965b3a4ac',
  'zlib_revision': 'f8517bd62931d7adb9bcefb0cbe3c2ca5cd8862c',
  'googletest_revision': '4fe018038f87675c083d0cfb6a6b57c274fb1753',
  'jinja2_revision': '61cfe2ac6c9108534c43b4039a95a0980251f266',
  'markupsafe_revision': 'f2fb0f21ef1e1d4ffd43be8c63fc3d4928dea7ab',
  'checkout_google_benchmark': False,
}

deps = {
  'build':
    Var('chromium_git') + '/chromium/src/build.git' + '@' + Var('build_revision'),
  'buildtools':
    Var('chromium_git') + '/chromium/src/buildtools.git' + '@' + Var('buildtools_revision'),
  'buildtools/mac': {
    'packages': [
      {
        'package': 'gn/gn/mac-amd64',
        'version': Var('gn_version'),
      }
    ],
    'dep_type': 'cipd',
    'condition': 'host_os == "mac"',
  },
  'base/trace_event/common':
    Var('chromium_git') + '/chromium/src/base/trace_event/common.git' + '@' + Var('trace_event_revision'),
  'buildtools/third_party/libc++/trunk':
    Var('chromium_git') + '/external/github.com/llvm/llvm-project/libcxx.git' + '@' + Var('libc++_revision'),
  'buildtools/third_party/libc++abi/trunk':
    Var('chromium_git') + '/external/github.com/llvm/llvm-project/libcxxabi.git' + '@' + Var('libc++abi_revision'),
  'tools/clang':
    Var('chromium_git') + '/chromium/src/tools/clang.git' + '@' + Var('clang_revision'),
  'v8':
    Var('chromium_git') + '/v8/v8.git' + '@' +  Var('v8_revision'),
  'third_party/icu':
    Var('chromium_git') + '/chromium/deps/icu.git' + '@' + Var('icu_revision'),
  'third_party/zlib':
    Var('chromium_git') + '/chromium/src/third_party/zlib.git'+ '@' + Var('zlib_revision'),
  'third_party/googletest/src':
    Var('chromium_git') + '/external/github.com/google/googletest.git' + '@' + Var('googletest_revision'),
  'third_party/jinja2':
    Var('chromium_git') + '/chromium/src/third_party/jinja2.git' + '@' + Var('jinja2_revision'),
  'third_party/markupsafe':
    Var('chromium_git') + '/chromium/src/third_party/markupsafe.git' + '@' + '',
}

hooks = [
  {
    # Note: On Win, this should run after win_toolchain, as it may use it.
    'name': 'clang',
    'pattern': '.',
    # clang not supported on aix
    'condition': 'host_os != "aix"',
    'action': ['python', 'tools/clang/scripts/update.py'],
  },
]
