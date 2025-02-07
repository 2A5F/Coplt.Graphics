namespace Coplt.Graphics.Native;

public partial struct FResourceSrc
{
    public FResourceSrc(uint index)
    {
        ResourceIndex = index;
    }
    public FResourceSrc(int index)
    {
        ResourceIndex = (uint)index;
    }
}
