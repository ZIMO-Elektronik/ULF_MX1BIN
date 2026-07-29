# Read / Set Command Station CV - **Code 12**

## Available Messages

### [Primary](../../definition/message_type/primary.md)

| Offset  | Section       | Size    | Field                       | Value             | Description                         |
| ------: | ------------: | ------: | --------------------------- | ----------------- | ----------------------------------- |
| 0       | Head          | 1 Byte  | uSID                        | -                 | -                                   |
| 1       | Head          | 1 Byte  | Header Info                 | -                 | -                                   |
| 2       | Head          | 1 Byte  | Message Code                | 12                | -                                   |
| 3       | Data          | 2 Byte  | Variable                    | -                 | CV address                          |
| 5       | OptionalData  | 1 Byte  | Value                       | -                 | If present, set as new CV-value     | 

### [Level 1 Ack](../../definition/message_type/level_1_ack.md)

| Offset  | Section       | Size    | Field                       | Value     | Description                       |
| ------: | ------------: | ------- | --------------------------- | --------- | --------------------------------- |
| 0       | Head          | 1 Byte  | uSID                        | -         | -                                 |
| 1       | Head          | 1 Byte  | Header Info                 | -         | -                                 |
| 2       | Head          | 1 Byte  | Message Code                | 12        | -                                 |
| 3       | Head          | 1 Byte  | pre-uSID                    | -         | -                                 |
| 4       | Data          | 1 Byte  | Error                       | -         | If any - Error                    | 
| 5       | Data          | 1 Byte  | cValue                      | -         | (New) Value of the CV             |