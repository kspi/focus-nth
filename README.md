# focus-nth

## Description

A simple X11 application that focuses a window based on its index in a
taskbar. This can be used to implement keybindings like Alt-1 selecting
the first window in the taskbar, Alt-2 the second and so on.

The zero-based window index is passed as the sole argument.

The „taskbar“ means a list of window icons and the window order is
decided by the vertical and horizontal position of window icons (from
the `_NET_WM_ICON_GEOMETRY` property). This is tested with the tint2
panel and Openbox.

## Example usage

Example Openbox rc.xml keybindings, assuming the program is in the PATH:

```xml
    <keybind key="A-1">
      <action name="execute">
        <execute>focus-nth 0</execute>
      </action>
    </keybind>
    <keybind key="A-2">
      <action name="execute">
        <execute>focus-nth 1</execute>
      </action>
    </keybind>
    <keybind key="A-3">
      <action name="execute">
        <execute>focus-nth 2</execute>
      </action>
    </keybind>
    <keybind key="A-4">
      <action name="execute">
        <execute>focus-nth 3</execute>
      </action>
    </keybind>
    <keybind key="A-5">
      <action name="execute">
        <execute>focus-nth 4</execute>
      </action>
    </keybind>
    <keybind key="A-6">
      <action name="execute">
        <execute>focus-nth 5</execute>
      </action>
    </keybind>
    <keybind key="A-7">
      <action name="execute">
        <execute>focus-nth 6</execute>
      </action>
    </keybind>
    <keybind key="A-8">
      <action name="execute">
        <execute>focus-nth 7</execute>
      </action>
    </keybind>
    <keybind key="A-9">
      <action name="execute">
        <execute>focus-nth 8</execute>
      </action>
    </keybind>
    <keybind key="A-0">
      <action name="execute">
        <execute>focus-nth 9</execute>
      </action>
    </keybind>
```


## License

focus-nth
Copyright (C) 2014 kspi

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
