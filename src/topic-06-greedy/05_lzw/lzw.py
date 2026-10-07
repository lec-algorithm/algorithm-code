"""LZW 압축 — 반복되는 문자열에 코드를 붙여 가며 압축한다.

인코더는 읽으면서 문자열 표를 만들고, 디코더는 코드만 보고 같은 표를
스스로 다시 만든다. 표는 보내지 않는다.

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 lzw.py`
"""


def lzw_encode(text):
    """text를 코드 목록으로 바꾼다. 걸음마다 한 줄 찍는다."""
    table = {chr(c): c for c in range(256)}  # 코드 0~255는 ASCII
    codes = []

    p = text[0]
    print(f"  {'P':<5} {'C':<3} {'P+C':<5} action")
    for c in text[1:]:
        pc = p + c
        print(f"  {p:<5} {c:<3} {pc:<5} ", end="")
        if pc in table:
            print(f"in table, P = {pc}")
            p = pc
        else:
            codes.append(table[p])
            table[pc] = len(table)
            print(f"out {codes[-1]} ({p}), add {table[pc]} = {pc}")
            p = c
    codes.append(table[p])
    print(f"  {p:<5} {'':<3} {'':<5} out {codes[-1]} ({p})")
    return codes


def lzw_decode(codes):
    """코드 목록을 원래 문자열로 되돌린다. 걸음마다 한 줄 찍는다."""
    table = {c: chr(c) for c in range(256)}

    old = codes[0]
    out = [table[old]]
    c = table[old][0]
    print(f"  {'OLD':<5} {'NEW':<5} {'S':<5} {'C':<3} add")
    print(f"  {'':<5} {'':<5} {table[old]}")
    for code in codes[1:]:
        known = code in table
        if not known:
            s = table[old] + c
        else:
            s = table[code]
        out.append(s)
        c = s[0]
        entry = table[old] + c
        added = len(table)
        table[added] = entry
        note = "" if known else "  (NEW not in table yet)"
        print(f"  {old:<5} {code:<5} {s:<5} {c:<3} {added} = {entry}{note}")
        old = code
    return "".join(out)


if __name__ == "__main__":
    text = "BABAABAAA"

    print(f"text = {text}\n\nencode")
    codes = lzw_encode(text)

    print("codes:", *codes)
    print("\ndecode")
    decoded = lzw_decode(codes)
    print(f"decoded = {decoded}")

    print(f"\nbits: ascii {len(text)} x 8 = {8 * len(text)},"
          f" lzw {len(codes)} x 9 = {9 * len(codes)}")
