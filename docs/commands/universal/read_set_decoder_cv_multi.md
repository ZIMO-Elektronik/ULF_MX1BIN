# Read / Set Multiple Decoder CVs - **Code 20**

## Available Messages

### [Primary](../../definition/message_type/primary.md)

| Offset  | Section       | Size    | Field                       | Value             | Description                         |
| ------: | ------------: | ------: | --------------------------- | ----------------- | ----------------------------------- |
| 0       | Head          | 1 Byte  | uSID                        | -                 | -                                   |
| 1       | Head          | 1 Byte  | Header Info                 | -                 | -                                   |
| 2       | Head          | 1 Byte  | Message Code                | 20                | -                                   |
| 3       | Data          | 2 Byte  | cAdr                        | ffaaaaaa aaaaaaaa | Decoder Address                     |
| 5       | Data          | 2 Byte  | Index                       | -                 | CV page index (CV31 CV32)           |
| 7       | Data          | 1 Byte  | Variable                    | -                 | CV index                            |
| 8       | Data          | 1 Byte  | SequenceID                  | \[0..3]           | Xpom SequenceID                     | 
| 8       | Optional Data | 1 Byte  | Value1                      | -                 | If present, set as new CV-value     | 
| 9       | Optional Data | 1 Byte  | Value2                      | -                 | If present, set as new CV-value     | 
| 10      | Optional Data | 1 Byte  | Value3                      | -                 | If present, set as new CV-value     | 
| 11      | Optional Data | 1 Byte  | Value4                      | -                 | If present, set as new CV-value     | 

---

### [Level 1 Ack](../../definition/message_type/level_1_ack.md)

| Offset  | Section       | Size    | Field                       | Value     | Description                       |
| ------: | ------------: | ------- | --------------------------- | --------- | --------------------------------- |
| 0       | Head          | 1 Byte  | uSID                        | -         | -                                 |
| 1       | Head          | 1 Byte  | Header Info                 | -         | -                                 |
| 2       | Head          | 1 Byte  | Message Code                | 20        | -                                 |
| 3       | Head          | 1 Byte  | pre-uSID                    | -         | -                                 |

---

### [Busy - Level 1 Ack](../../definition/message_type/level_1_ack.md)

| Offset  | Section       | Size    | Field                       | Value             | Description                           |
| ------: | ------------: | ------- | --------------------------- | ----------------- | ------------------------------------- |
| 0       | Head          | 1 Byte  | uSID                        | -                 | -                                     |
| 1       | Head          | 1 Byte  | Header Info                 | -                 | -                                     |
| 2       | Head          | 1 Byte  | Message Code                | 20                | -                                     |
| 3       | Head          | 1 Byte  | pre-uSID                    | -                 | -                                     |
| 4       | Data          | 1 Byte  | cBusy                       | 4                 | Unable to handle query ATM            |
| 5       | Data          | 2 Byte  | cAdr                        | ffaaaaaa aaaaaaaa | Decoder Address                       |
| 7       | Data          | 2 Byte  | Index                       | -                 | CV page index                         | 
| 9       | Data          | 1 Byte  | Variable                    | -                 | CV index                              |
| 10      | Data          | 1 Byte  | SequenceID                  | -                 | Xpom SequenceID                       |  
| 11      | Optional Data | 1 Byte  | cID                         | -                 | ID of the blocking query              |
| 12      | Optional Data | 1 Byte  | cError                      | -                 | Error                                 |
| 13      | Optional Data | 2 Byte  | cAdr                        | ffaaaaaa aaaaaaaa | Decoder Address of the blocking query |
| 15      | Optional Data | 2 Byte  | Index                       | -                 | CV page index of the blocking query   | 
| 17      | Optional Data | 2 Byte  | Variable                    | -                 | CV index of the blocking query        | 
| 18      | Optional Data | 1 Byte  | SequenceID                  | \[0..3]           | Xpom SequenceID of the blocking query |

---

### [Reply Level 2](../../definition/message_type/reply_level_2.md)

| Offset  | Section       | Size    | Field                       | Value             | Description                           |
| ------: | ------------: | ------- | --------------------------- | ----------------- | ------------------------------------- |
| 0       | Head          | 1 Byte  | uSID                        | -                 | -                                     |
| 1       | Head          | 1 Byte  | Header Info                 | -                 | -                                     |
| 2       | Head          | 1 Byte  | Message Code                | 20                | -                                     |
| 3       | Head          | 1 Byte  | pre-uSID                    | -                 | -                                     |
| 4       | Data          | 2 Byte  | cAdr                        | ffaaaaaa aaaaaaaa | Decoder Address                       | 
| 6       | Data          | 2 Byte  | Index                       | -                 | CV page index                         | 
| 8       | Data          | 1 Byte  | Variable                    | -                 | CV index                              | 
| 9       | Data          | 1 Byte  | SequenceID                  | \[0..3]           | Xpom SequenceID                       |
| 10      | Data          | 1 Byte  | cError                      | -                 | Error                                 | 
| 11      | Data          | 1 Byte  | cValue1                     | -                 | First CV Value                        | 
| 12      | Data          | 1 Byte  | cValue2                     | -                 | Second CV Value                       | 
| 13      | Data          | 1 Byte  | cValue3                     | -                 | Third CV Value                        | 
| 14      | Data          | 1 Byte  | cValue4                     | -                 | Fourth CV Value                       | 

---

### [Error - Reply Level 2](../../definition/message_type/reply_level_2.md)

| Offset  | Section       | Size    | Field                       | Value             | Description                           |
| ------: | ------------: | ------- | --------------------------- | ----------------- | ------------------------------------- |
| 0       | Head          | 1 Byte  | uSID                        | -                 | -                                     |
| 1       | Head          | 1 Byte  | Header Info                 | -                 | -                                     |
| 2       | Head          | 1 Byte  | Message Code                | 20                | -                                     |
| 3       | Head          | 1 Byte  | pre-uSID                    | -                 | -                                     |
| 4       | Data          | 2 Byte  | cAdr                        | ffaaaaaa aaaaaaaa | Decoder Address                       | 
| 6       | Data          | 1 Byte  | cError                      | -                 | Error                                 |

---

### [Level 2 Ack](../../definition/message_type/level_2_ack.md)

| Offset  | Section       | Size    | Field                       | Value     | Description                       |
| ------: | ------------: | ------- | --------------------------- | --------- | --------------------------------- |
| 0       | Head          | 1 Byte  | uSID                        | -         | -                                 |
| 1       | Head          | 1 Byte  | Header Info                 | -         | -                                 |
| 2       | Head          | 1 Byte  | Message Code                | 20        | -                                 |
| 3       | Head          | 1 Byte  | pre-uSID                    | -         | -                                 |

---