;    From Jacques.Herry@wanadoo.fr 
;            Jacques Herry 
;           35, resid Corlay 
;           22300 Lannion 
;            France 
;  	Grame  MidiShare for schemers. 
;  "Welcome to Grame MidiShare(c)extension player, version 1.6.22 Copyright(c)2001 Jacques Herry" 


Install the midishare library as normal installation.
Verify that midishare run ok with Grame's applications before use my libs.

extract mzshare.zip on a directory of your choice for example yourPath>

Get the grame midiplayer 2.0 from ftp. (put this lib on your path)
Warning : The borland lib must be also in your path : BC450RTL.DLL and BWCC.DLL

Load my foreign lib  with (load-extension "yourPath/mzshare3216")
			  (load-extension "yourPath/mzplayer3216")



Some bugs can persist  but you have the src and you are free to change
anythings (with changes history commented please) and you are free to 
use this lib under the same licences as (and Grame-licence  mzscheme-licence) => free (Grame+LGPL)



J'aime Scheme play ;)