
  StatSVN README
  ==============

      StatSVN is a statistics tool for SVN repositories. It generates
      HTML reports from SVN log files.


  The StatSVN Manual

      The StatSVN manual is located here: http://svn.statsvn.org/statsvnwiki/index.php/UserManual


  Quick Start

      * Download the latest release from http://sourceforge.net/projects/statsvn/
      * Expand the zip file into some directory, e.g c:\statsvn
      * Check out a working copy of the desired SVN module into
        some directory, e.g. c:\myproject.
      * Change into that directory and type
        'svn log --xml -v > svn.log'
      * Change back to the c:\statsvn directory
      * type 'java -jar statsvn.jar c:\myproject\svn.log c:\myproject'
      * Open c:\statsvn\index.html in your web browser

      You can tweak the output of StatSVN in various ways. Run
      'java -jar statsvn.jar' for an overview of the command line
      parameters, and check the manual for full information.
