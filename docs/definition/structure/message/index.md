# Message

The message of the stream contains the data of a command. This can be either input or output. The structure
of data depends on a combination of [Message Type](../../message_type/index.md),
[Command Byte](../header/index.md#command-code) and the actual length, since optional data may be present

## Sections

A message may have multiple sections of data (excluding the [Header](../header/index.md)). An exception to
this rule are Header-Only messages (e.g. [Reset](../../../commands/universal/reset.md))

### Data

In the data section any in- or outgoing data that **_MUST_** be present will be placed. If expected data is
missing, it will result in parsing errors.

### Optional Data

Some messages have fields, that may or may not be present.In most cases this depends on wether an error
has occurred, but may also be dependant on the device that is connected (e.g. MXULF).

!!! warning "Consistency"

    Not all optional needs to be present **_BUT_** any optional data until the last optional byte needed
    must be present. As an example using [LocoControl](../../../commands/universal/loco_control.md#primary)
    If the last byte needed is `cData2`, `cData1` must also be present, otherwise parsing will most
    definetly fail.

    This however means, that cData1 needs to have an actual valid value, as it will be processed accordingly