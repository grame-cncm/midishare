This is the current state of the MidiShare project. The project is published
under the GNU Library General Public License. You should have received a copy 
of the GNU LGPL along with this distribution; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

The MidiShare kernel development documentation is located in the DevGuide folder.


Source code general organization
--------------------------------
- header files are grouped together in the 'Header' directory.
- platform specific functions (to implement) are defined in the 'msExtern.h' file.
- source files are grouped in separate directories, as independant kernel components.
  Each component uses its own independant data structures. The kernel itself provides
  the glue between all the components and specific services (at present time management 
  and in the future, drivers management).


Not yet implemented
-------------------
- smpte synchronization management.
- drivers management


Kernel development rules
------------------------
- you should susbscribe to the 'midishare-kernel' mailing list to be informed of the
  current developments. This mailing list is also the place where are discussed the
  kernel changes and improvements.
  To subscribe, send the following:
     subscribe midishare-kernel <optionnal email>
  in the body of a message to majordomo@rd.grame.fr

- modifications, improvements or extensions of the source code should be discussed and
  posted to the midishare-kernel mailing list.

- specific changes should be presented in the form of input to the patch command. 
  The patch should be created by using the diff -u command from the original file(s) 
  to the modified file(s). E.g.,
      diff -u file.c.orig file.c >> patchfile.txt
  or 
      cvs diff -u file.c >> patchfile.txt
  The patchfile should produce no errors or prompts when the command,
      patch -s < patchfile
  is issued in the target repository. 
