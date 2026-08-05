# CRC

The CRC for each frame is calculated over the encoded data, not including the Prefix / Suffix. If the CRC contains escape characters, they are encoded as well.

Short frames contain a CRC8. Init: `0xFF`   Poly: **_x<sup>8</sup> + x<sup>5</sup> + x<sup>4</sup> + 1_**

Long frames contain a CRC16. Init: `0xFFFF` Poly: **_x<sup>16</sup> + x<sup>12</sup> + x<sup>5</sup> + 1_**
