# Read / Set Command Station CV - **Code 12**

Some older Command Station have internal CV values, that can be read and written

## Available Messages

### [Primary](../../definition/message_type/primary.md)

| Offset |      Section |   Size | Field                | Value | Description                     |
| -----: | -----------: | -----: | -------------------- | ----- | ------------------------------- |
|      0 |         Head | 1 Byte | [Unique Sequence-ID] | -     | -                               |
|      1 |         Head | 1 Byte | [Header Info]        | -     | -                               |
|      2 |         Head | 1 Byte | [Command Code]       | 12    | -                               |
|      3 |         Data | 2 Byte | Variable             | -     | CV address                      |
|      5 | OptionalData | 1 Byte | Value                | -     | If present, set as new CV-value |

!!! info "The given `Variable` is read if the `Value` byte is ommitted. Otherwise the `Variable` is set to `Value`"

### [Level 1 Ack](../../definition/message_type/level_1_ack.md)

| Offset | Section | Size   | Field                                   | Value | Description           |
| -----: | ------: | ------ | --------------------------------------- | ----- | --------------------- |
|      0 |    Head | 1 Byte | [Unique Sequence-ID]                    | -     | -                     |
|      1 |    Head | 1 Byte | [Header Info]                           | -     | -                     |
|      2 |    Head | 1 Byte | [Command Code]                          | 12    | -                     |
|      3 |    Head | 1 Byte | [Unique Sequence-ID] of primary message | -     | -                     |
|      4 |    Data | 1 Byte | Error                                   | -     | If any - Error        |
|      5 |    Data | 1 Byte | cValue                                  | -     | (New) Value of the CV |

---

[Command Code]: ../../definition/structure/header/index.md#command-code
[Header Info]: ../../definition/structure/header/index.md#header-info
[Unique Sequence-ID]: ../../definition/structure/header/index.md#unique-sequence-id