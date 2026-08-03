# Reset - **Code 1**

The `Reset` message is used to reset the protocol state. This includes resetting the Sequence ID counter, as well as deleting any pending messages etc.

## Available Messages

### [Primary](../../definition/message_type/primary.md)

!!! info "Header only"

| Offset | Area |   Size | Field                | Value |
| -----: | ---: | -----: | -------------------- | ----- |
|      0 | Head | 1 Byte | [Unique Sequence-ID] | -     |
|      1 | Head | 1 Byte | [Header Info]        | -     |
|      2 | Head | 1 Byte | [Command Code]       | 1     |

---

### [Level 1 Ack](../../definition/message_type/level_1_ack.md)

!!! info "Header only"

| Offset | Area |   Size | Field                                   | Value |
| -----: | ---: | -----: | --------------------------------------- | ----- |
|      0 | Head | 1 Byte | [Unique Sequence-ID]                    | -     |
|      1 | Head | 1 Byte | [Header Info]                           | -     |
|      2 | Head | 1 Byte | [Command Code]                          | 1     |
|      3 | Head | 1 Byte | [Unique Sequence-ID] of primary message | -     |

---

[Command Code]: ../../definition/structure/header/index.md#command-code
[Header Info]: ../../definition/structure/header/index.md#header-info
[Unique Sequence-ID]: ../../definition/structure/header/index.md#unique-sequence-id