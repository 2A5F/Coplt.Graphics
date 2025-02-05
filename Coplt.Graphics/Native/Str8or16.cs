namespace Coplt.Graphics.Native;

public unsafe partial struct Str8or16
{
    public Str8or16(string? name, ReadOnlySpan<byte> name8, char* p_name, byte* p_name8)
    {
        str8 = name != null ? null : name8.Length > 0 ? p_name8 : null;
        str16 = p_name;
        len = name?.Length ?? name8.Length;
    }
}
