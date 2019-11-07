# DebugReport

DebugReport is used to report some message of `CodeRed`. The info, warning, error message are sent from `DebugReport` to all listeners. And the listeners can store the messages or output the messages to console, log window and so on.

## DebugListener

A debug listener can receive all messages sent from `DebugReport`. In general, we use debug listener to output the messages to user.

A debug listener should inherit from `DebugListener` class and implement the `DebugListener::receive` method. The `DebugStdListener` is a example of debug listener and it is the default listener.

## Add Listener to Report

`DebugReport::listeners()` return all listeners that we are using and you can remove or add any listeners with this mehtod.

```C++
    DebugReport::listeners().push_back(std::make_shared<...>(...))
```

