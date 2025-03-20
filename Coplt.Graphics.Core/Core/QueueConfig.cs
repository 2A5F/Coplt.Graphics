namespace Coplt.Graphics.Core;

public record struct QueueConfig
{
    /// <summary>
    /// 额外的直接队列，默认保证有一个主队列
    /// </summary>
    public uint NumDirect;
    public uint NumCompute;
    public uint NumCopy;
    public uint NumVideoEncode;
    public uint NumVideoDecode;
    public uint NumVideoProcess;
}
