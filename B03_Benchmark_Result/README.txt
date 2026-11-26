B03 Benchmark Images

Source: two supplied logs (x64 Debug and x64 Release).
Each build: 192 runs / 96 paired conditions / 10,560,000 successful transfers / 0 failures.
All 384 observations retained. No smoothing, filtering or averaging.

Debug/ and Release/ each contain six complete tables (16 paired conditions each), four full metric heatmaps (ms and TPS for each lock), and one PerAccount/Global TPS ratio heatmap.
Comparison/ contains the summary, four selected-slice comparisons (both builds, both metrics), and two Release/Debug ratio heatmaps covering every condition.

TPS = EventCount / (milliseconds / 1000). Source TPS values are rounded by the original console output. Heatmaps and plot labels are rounded for readability; full tables retain logged precision.
A ratio >1 indicates numerator throughput is higher. Metric heatmaps share a scale across builds and lock types. Ratio heatmaps use log2 color normalization.
Each condition is measured only once. Timing includes thread creation and join according to the previously supplied harness. HotAccountRatio 0% excludes account 0 in the supplied request generator.
Do not interpret the observed timings as statistically robust; repeated warm-up and measurement are recommended.
