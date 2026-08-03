# Reply Level 2

A message of this type follows the end of an asynchronous operation and contains its result, wether successful or not. This message _MUST_ be replied with a [Level 2 Ack](level_2_ack.md).

``` mermaid
sequenceDiagram
    participant PC
    participant CommandStation

    PC->>CommandStation: Primary
    CommandStation-->>PC: Level 1 Ack
    CommandStation->>PC: Reply Level 2
    Note right of CommandStation: You are here!
    PC-->>CommandStation: Level 2 Ack
```