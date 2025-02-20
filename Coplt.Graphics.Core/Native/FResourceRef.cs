namespace Coplt.Graphics.Native;

public partial struct FResourceRef
{
    public FResourceRef(uint index)
    {
        ResourceIndex = index;
    }
    public FResourceRef(int index)
    {
        ResourceIndex = (uint)index;
    }
}
