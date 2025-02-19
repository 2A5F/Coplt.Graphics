using Coplt.Graphics.Native;
using Serilog.Events;

namespace Examples;

public static class Utils
{
    public static LogEventLevel ToLogEventLevel(this FLogLevel level) => level switch
    {
        FLogLevel.Fatal => LogEventLevel.Fatal,
        FLogLevel.Error => LogEventLevel.Error,
        FLogLevel.Warn => LogEventLevel.Warning,
        FLogLevel.Info => LogEventLevel.Information,
        FLogLevel.Debug => LogEventLevel.Debug,
        FLogLevel.Trace => LogEventLevel.Verbose,
        _ => throw new ArgumentOutOfRangeException(nameof(level), level, null)
    };
}
