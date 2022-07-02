# Logitech-NumLock-Indicator

An optimization focused background process (doens't even live in tray) that switches the rgb backlight of the numlock key when numlock is toggled.
(Some logitech keyboards don't have a numlock led for whatever inane reason)

This uses the Logitech LED library (requires G hub) and an interrupt based paradigm. When a keyboard key is pressed, the callback function is called, and if it is
the numlock key, a new thread is spun to get the toggle state and change the led. Personal testing shows:

0% cpu usage when nothing is happening

0% when the keyboard buttons other than numlock are spammed (the callback function is called, so i guess it takes less usage than task manager is capable of showing aka <0.1%)

0.1% to 1.7% when the numlock key is spammed.


I've hardcoded the keyboard to be red, with the numlock being either off or blue-ish. 

It can be scheduled to run on start up (with few second delay so ghub can launch) or 
use task scheduler to have it trigger when ghub launches in general (also few second delay).

Run it with elevated permissions (Administrator) otherwise the global hook won't go through other elevated programs like Task Manager.

It's all hardcoded and the automation above requires using window's task scheduler since it's for myself, but if people stumble on this and want something more polished lmk. 

Other avenues that would be interesting to investigate are dll-hijacking G-hub or modifying the keyboard firmware directly so the numlock indicator could be independent of the ghub led sdk, and the computer in general.
