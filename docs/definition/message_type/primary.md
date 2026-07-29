# Primary Message

The primary message serves as starting point for each command. Wether the command can be processed immediatly or will be deferred, a [Level 1 Ack](level_1_ack.md) **MUST** follow

```mermaid
sequenceDiagram
    participant PC
    participant CommandStation

    PC->>CommandStation: Primary
    Note left of PC: You are here!
    CommandStation-->>PC: Level 1 Ack
    CommandStation->>PC: Reply Level 2
    PC-->>CommandStation: Level 2 Ack
```