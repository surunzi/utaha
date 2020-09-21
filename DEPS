use_relative_paths = True

gclient_gn_args_file = 'build/config/gclient_args.gni'
gclient_gn_args = [
  'checkout_google_benchmark',
]

vars = {
  'chromium_git': 'https://chromium.googlesource.com',
  'build_revision': '153ad0bf09eda458f1ef4f74dcac5c12d530d770',
  'buildtools_revision': '204a35a2a64f7179f8b76d7a0385653690839e21',
  'gn_version': 'git_revision:e002e68a48d1c82648eadde2f6aafa20d08c36f2',
  'v8_revision': '80dc1e8055bca07e8da6162bace5ee87f23faccf',
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
  'v8':
    Var('chromium_git') + '/v8/v8.git' + '@' +  Var('v8_revision'),
}

