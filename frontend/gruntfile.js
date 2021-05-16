module.exports = function (grunt) {

  require('load-grunt-tasks')(grunt);

  grunt.loadNpmTasks('grunt-contrib-clean');
  grunt.loadNpmTasks('grunt-contrib-copy');
  grunt.loadNpmTasks('grunt-contrib-uglify');
  grunt.loadNpmTasks('grunt-mkdir');
  grunt.loadNpmTasks('grunt-contrib-concat');


  grunt.initConfig({

    useminPrepare: {
      html: 'app/index.html',
      options: {
        dest: 'dist/'
      }
    },

    usemin: {
      html: ['dist/index.html'],
      options: {
        assetsDirs: ['dist/'],
      }
    },
    clean: {
      dist: ["dist/**"] //clean build means clean the build directory
    },
    mkdir: {
      //mkdir build means create a scripts folder under build/
      dist: {
        options: {
          create: ['dist/scripts', 'dist/styles', 'div/images']
        }
      }
    },
    copy: {
      dist: {
        files: [{
          expand: true,
          cwd: "app/views/",
          src: "**",
          dest: "dist/views"
        },
          {
            expand: true,
            cwd: "app/fonts/",
            src: "**",
            dest: "dist/fonts"
          },
          {
            expand: true,
            cwd: "app/images/",
            src: "**",
            dest: "dist/images"
          },
          {
            expand: true,
            cwd: "app/3d_assets/",
            src: "**",
            dest: "dist/3d_assets"
          },
          {
            expand: false,
            src: "app/index.html",
            dest: "dist/index.html"
          }
        ]
      },
      generated: {
        files: [
          {
            expand: true,
            cwd: ".tmp/concat/scripts",
            src: "**",
            dest: "dist/scripts/"
          }
          /*
          ,
          {
            expand: true,
            cwd: ".tmp/concat/styles",
            src: "**",
            dest: "dist/css/"
          }
          */
        ]
      }
    }
  });
  //replace uglify:generated with copy:generated when it turns out which lib fails to be minified from the vendor scripts
  grunt.registerTask('build', ['clean:dist', 'mkdir:dist', 'copy:dist', 'useminPrepare', 'concat:generated', 'cssmin:generated', 'copy:generated', 'usemin']);

};
