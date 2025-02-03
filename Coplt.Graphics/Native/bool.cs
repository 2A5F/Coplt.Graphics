namespace Coplt.Graphics.Native;

public readonly record struct B8(byte Value)
{
    public static implicit operator bool(B8 value) => value.Value != 0;
    public static implicit operator B8(bool value) => new(value ? (byte)1 : (byte)0);

    public static bool operator true(B8 value) => value.Value != 0;
    public static bool operator false(B8 value) => value.Value == 0;

    public override string ToString() => this ? "true" : "false";
}

public readonly record struct B16(ushort Value)
{
    public static implicit operator bool(B16 value) => value.Value != 0;
    public static implicit operator B16(bool value) => new(value ? (ushort)1 : (ushort)0);

    public static bool operator true(B16 value) => value.Value != 0;
    public static bool operator false(B16 value) => value.Value == 0;

    public override string ToString() => this ? "true" : "false";
}
