# Level 1 Ack

The `Level 1 Ack` marks the end of the synchronous part of command processign (and the end of most commands). The sender simply assumes that this type of message is always received.

```mermaid
sequenceDiagram
    participant PC
    participant CommandStation

    PC->>CommandStation: Primary
    CommandStation-->>PC: Level 1 Ack
    Note right of CommandStation: You are here!
    CommandStation->>PC: Reply Level 2
    PC-->>CommandStation: Level 2 Ack
```