CGM-Editor


usage: cgm-editor <cgm-file> [--size x y]

Tthe editor must be called with one filename to edit.

I added the file console.cmd to easily open a console
to call the editor with parameters.

e.g.:
cgm-editor levels/test.cgm

You can resize a level by giving a new size through
the --size parameter. If you shrink the level, the left
front part will stay. If you enlage it, it will be expanded
to the right back.

e.g.:
cgm-editor levels/test.cgm --size 20 20

To create a new level yust give a filename that does not
exists and the size of the new level. the file will
be created if you save.

e.g.:
cgm-editor levels/newlevel.cgm --size 20 20

To test your new level make a backup of levels/default.lev
and replace its contend with the filename of your new file.

e.g.:
contend of levels/default.lev:
levels/newlevel.cgm

And add a blank line at the end of this file.

There is a trick if you run the game and the editor parallel.
If you reached the finish, you can edit the game and press
"Play Again" to play the edited level.
