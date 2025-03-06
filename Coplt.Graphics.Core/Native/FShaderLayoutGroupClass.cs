namespace Coplt.Graphics.Native;

public unsafe partial struct FShaderLayoutGroupClass
{
    public ReadOnlySpan<FShaderLayoutGroupInfo> InfoSpan => new(Infos, (int)Size);
}
