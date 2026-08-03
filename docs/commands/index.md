# Commands

The protocol defines a list of commands, that may or may not be executed by a command station. Since the protocol grew over time, there are categories (or types) of commands that are either available universally or per command station type.

## List of commands

Below is an incomplete list available of MX1Bin commands.

### Universal commands

Commands that are available on any type of command station

!!! warning "Some devices (e.g. [`KLUG`](https://www.zimo.at/web2010/products/KLUG-Kompaktes_Lade_und_Update-Geraet.htm), [`MXULF`](https://www.zimo.at/web2010/products/MXULF-Decoder-Updater.htm)) only implement a subset of these"

- **[Reset](universal/reset.md)**
- **[Nak](universal/nak.md)**
- **[Track Control](universal/acceleration_deceleration.md)**
- **[Loco Control](universal/loco_control.md)**
- **[Invert Function Bits](universal/invert_function_bits.md)**
- **[Acceleration Deceleration](universal/acceleration_deceleration.md)**
- **[Shuttle Train](universal/shuttle_train.md)**
- **[Accessory Control](universal/accessory_control.md)**
- **[Loco Memory Query](universal/loco_memory_query.md)**
- **[Accessory Memory Query](universal/accessory_memory_query.md)**
- **[Address Control](universal/address_control.md)**
- **[Read Command Station I/O State](universal/read_command_station_io_state.md)**
- **[Read / Set Command Station CV](universal/read_set_command_station_cv.md)**
- **[Command Station Equipment Query](universal/command_station_equipment_query.md)**
- **[Read / Set Decoder CV](universal/read_set_decoder_cv.md)**
- **[Read / Set Multiple Decoder CVs](universal/read_set_decoder_cv_multi.md)**
- **[Current Loco Memory](universal/current_loco_memory.md)**
- **[Current Accessory Memory](universal/current_accessory_memory.md)**