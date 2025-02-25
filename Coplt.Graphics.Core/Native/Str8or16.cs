namespace Coplt.Graphics.Native;

public unsafe partial struct FStr8or16
{
    public FStr8or16(byte* p_name8, int size)
    {
        type = FStrType.Str8;
        str8 = p_name8;
        len = size;
    }

    public FStr8or16(char* p_name, int size)
    {
        type = FStrType.Str16;
        str16 = p_name;
        len = size;
    }

    public FStr8or16(string? name, ReadOnlySpan<byte> name8, char* p_name, byte* p_name8)
    {
        type = name == null && name8.Length > 0 ? FStrType.Str8 : FStrType.Str16;
        switch (type)
        {
        case FStrType.Str16:
            str16 = p_name;
            len = name?.Length ?? 0;
            break;
        case FStrType.Str8:
            str8 = p_name8;
            len = name8.Length;
            break;
        default:
            throw new ArgumentOutOfRangeException();
        }
    }
}
