import struct

def pack_command(vx, vy, omega):
    packet_data = struct.pack('<fff', vx, vy, omega)

    checksum = 0
    for byte in packet_data:
        checksum ^= byte

    final_packet = b'\xA5' + packet_data + bytes([checksum])
    return final_packet

if __name__ == "__main__":
    data = pack_command(1.5, 0.0, 0.0)
    print(f"Packet bytes: {data.hex()}")
    print(f"Panjang Packet: {len(data)} bytes")
