# Structure

The Stucture of the `MX1Bin` protocol essentially consists of two layers. The [Transport](stream/index.md) layer,
which specifies how a stream of bytes is to be decoded / encoded, and the [Message](message/index.md) layer, which
defines the structure and content of a distinct message.

Since the protocol as a whole is context sensitive, the parsing context is provided by a [Header](header/index.md)
and each frame is secured with a [Checksum](crc/index.md)
