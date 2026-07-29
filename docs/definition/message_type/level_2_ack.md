# Level 2 Ack

```mermaid
sequenceDiagram
    participant PC
    participant CommandStation

    PC->>CommandStation: Primary
    CommandStation-->>PC: Level 1 Ack
    CommandStation->>PC: Reply Level 2
    PC-->>CommandStation: Level 2 Ack
    Note left of PC: You are here!
```