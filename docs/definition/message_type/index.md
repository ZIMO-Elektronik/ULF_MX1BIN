# Message Types

In theory, the protocol defines a maximum of four phases for a command. For most messages,
only the [Primary](primary.md) and [Level 1 Ack](level_1_ack.md) are defined, however, for
some commands, asynchronous operations are necessary, which will be handled by
[Reply level 2](reply_level_2.md) and [Level 2 Ack](level_2_ack.md) respectively.

## List of types

- **[Primary](primary.md)** - The primary message for a command
- **[Level 1 Ack](level_1_ack.md)** - The ack matching the primary message
- **[Reply level 2](reply_level_2.md)** - The result of an async operation (if any)
- **[Level 2 Ack](level_2_ack.md)** - The ack for the async result (if any)