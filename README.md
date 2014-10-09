# focus-nth

A simple X11 application that focuses a window based on its index in a
taskbar. This can be used to implement keybindings like Alt-1 selecting
the first window in the taskbar, Alt-2 the second and so on.

The zero-based window index is passed as the sole argument.

The „taskbar“ means a list of window icons and the window order is
decided by the vertical and horizontal position of window icons (from
the `_NET_WM_ICON_GEOMETRY` property). This is tested with the tint2
panel and Openbox.

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
