import struct
import os

def convert_to_tiny_bin(input_path, output_path):
    vertices = []
    normals = []
    faces = []

    with open(input_path, 'r') as f:
        for line in f:
            p = line.split()
            if not p: continue
            if p[0] == 'v':
                vertices.append([float(x) for x in p[1:4]])
            elif p[0] == 'vn':
                normals.append([float(x) for x in p[1:4]])
            elif p[0] == 'f':
                f_indices = []
                for v_def in p[1:4]:
                    parts = v_def.split('/')
                    v_idx = int(parts[0]) - 1
                    # Falls keine Normalen da sind, Index 0 nutzen
                    n_idx = int(parts[-1]) - 1 if len(parts) > 2 else 0
                    f_indices.append((v_idx, n_idx))
                faces.append(f_indices)

    with open(output_path, 'wb') as f:
        # Header: v_count(H), n_count(H), f_count(H) -> nur 6 Byte!
        f.write(struct.pack('HHH', len(vertices), len(normals), len(faces)))
        
        for v in vertices:
            f.write(struct.pack('fff', *v))
        for n in normals:
            f.write(struct.pack('fff', *n))
        for face in faces:
            # v1, v2, v3, n1, n2, n3 als 16-bit
            f.write(struct.pack('HHHHHH', 
                face[0][0], face[1][0], face[2][0],
                face[0][1], face[1][1], face[2][1]))

    print(f"Abgeschlossen: {output_path} ({os.path.getsize(output_path)} Bytes)")

# Beispielaufruf
convert_to_tiny_bin('C:\\Users\\segerj\\Documents\\cpp\\FlappyCube\\resources\\cube_character.obj', 'C:\\Users\\segerj\\Documents\\cpp\\FlappyCube\\resources\\cube_character.fcm')