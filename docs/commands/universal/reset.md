# Reset - **Code 1**

The `Reset` message is used to reset the protocol state. This includes resetting the Sequence ID counter, as well as deleting any pending messages etc. 

## Available Messages

### [Primary](../../definition/message_type/primary.md)

!!! info "Header only"

| Offset  | Area | Size    | Field         | Value |
| ------: | ---: | ------: | ------------- | -     |
| 0       | Head | 1 Byte  | uSID          | -     |
| 1       | Head | 1 Byte  | Header Info   | -     |
| 2       | Head | 1 Byte  | Message Code  | 1     |

---

### [Level 1 Ack](../../definition/message_type/level_1_ack.md)

!!! info "Header only"

| Offset  | Area | Size    | Field         | Value |
| ------: | ---: | ------: | ------------- | -     |
| 0       | Head | 1 Byte  | uSID          | -     |
| 1       | Head | 1 Byte  | Header Info   | -     |
| 2       | Head | 1 Byte  | Message Code  | 1     |
| 3       | Head | 1 Byte  | pre-uSID      | -     | 

---