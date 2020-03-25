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

## Vulkan Validation Layer

When you enable `_DEBUG` macro, the vulkan validation layer will be loaded. It will write the messages to console. And the same messages we will only write once.

### Ignored Messages

Some messages from validation layer will be ignored. There are the all type of messages will be ignored.

- InitLayout of image must be `Undefined`. The default layout of texture is `ResourceLayout::GeneralRead` in Code-Red.
- Update a descriptor set with empty descriptor. When you set a descriptor heap to pipeline and textures the heap need are not bound.