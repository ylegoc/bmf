{
  "targets": [
    {
        "target_name": "addonbmf",
        'cflags!': [ '-fno-exceptions' ],
        'cflags_cc!': [ '-fno-exceptions' ],
        "sources": [
          "addons/bmf.cc",
        ],
        "conditions": [
            ['OS=="mac"', {
                'xcode_settings': {
                    'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
                },
            }]    
        ],
        "libraries": [
            "-lbmf"
        ]
    }

  ]
}
