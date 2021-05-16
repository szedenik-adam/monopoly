// Karma configuration
// Generated on Thu Oct 08 2015 09:14:14 GMT+0200 (CEST)

module.exports = function(config) {
  config.set({

    // base path that will be used to resolve all patterns (eg. files, exclude)
    basePath: '',


    // frameworks to use
    // available frameworks: https://npmjs.org/browse/keyword/karma-adapter
    frameworks: ['jasmine'],


    // list of files / patterns to load in the browser
    files: [
      'node_modules/angular/angular.js',
      'node_modules/angular-animate/angular-animate.js',
      'node_modules/angular-bootstrap/ui-bootstrap.js',
      'node_modules/angular-websocket/angular-websocket.js',
      'node_modules/angular-toastr/dist/angular-toastr.js',
      'node_modules/angular-websocket/angular-websocket.js',
      'node_modules/angular-mocks/angular-mocks.js',
      'node_modules/angular-ui-router/build/angular-ui-router.js',
      'app/scripts/controllers/module.js',
      'app/scripts/controllers/*.js',
      'app/scripts/directives/module.js',
      'app/scripts/directives/*.js',
      'app/scripts/models/module.js',
      'app/scripts/models/*.js',
      'app/scripts/services/module.js',
      'app/scripts/services/*.js',
      'app/scripts/app.js',
      'tests/**/*-Spec.js'
    ],


    // list of files to exclude
    exclude: [
    ],


    // preprocess matching files before serving them to the browser
    // available preprocessors: https://npmjs.org/browse/keyword/karma-preprocessor
    preprocessors: {
    },


    // test results reporter to use
    // possible values: 'dots', 'progress'
    // available reporters: https://npmjs.org/browse/keyword/karma-reporter
    reporters: ['progress'],


    // web server port
    port: 9876,


    // enable / disable colors in the output (reporters and logs)
    colors: true,


    // level of logging
    // possible values: config.LOG_DISABLE || config.LOG_ERROR || config.LOG_WARN || config.LOG_INFO || config.LOG_DEBUG
    logLevel: config.LOG_INFO,


    // enable / disable watching file and executing tests whenever any file changes
    autoWatch: true,


    // start these browsers
    // available browser launchers: https://npmjs.org/browse/keyword/karma-launcher
    browsers: ['Chrome'],


    // Continuous Integration mode
    // if true, Karma captures browsers, runs the tests and exits
    singleRun: false
  })
}
