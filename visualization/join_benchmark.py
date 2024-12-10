# Disclaimer: Created using ChatGPT
import matplotlib.pyplot as plt
import seaborn as sns

from helper import load_results_benchmark_directory_to_pandas, DATA_DIR


def plot_join_benchmark(metrics, labels):
    df = load_results_benchmark_directory_to_pandas(
        f"{DATA_DIR}/join_benchmark_different_build_sizes"
    )

    unique_ids = df["id"].unique()
    join_algos = df["config.algo"].unique()

    fig, axes = plt.subplots(
        len(metrics),
        len(unique_ids),
        figsize=(20, 8),
        sharex=True,
    )

    if len(unique_ids) == 1:
        axes = [axes]

    for y, (metric, label) in enumerate(zip(metrics, labels)):
        for i, unique_id in enumerate(unique_ids):

            ax = axes[y][i]
            filtered_df = df[df["id"] == unique_id]

            sns.barplot(
                data=filtered_df,
                x="config.build_size",
                y=metric,
                hue="config.algo",
                ax=ax,
                zorder=2,
                palette="pastel",
                edgecolor="black",
            )

            ax.set_title(f"{unique_id}")
            if y == len(metrics) - 1:
                ax.set_xlabel("Build Size", fontsize=15)
            if i == 0:
                ax.set_ylabel(label, fontsize=15)
            else:
                ax.set_ylabel(" ")
            ax.set_yscale("log")
            ax.grid(True, zorder=0, axis="y")
            if not (i == 0 and y == 0):
                ax.legend([], [], frameon=False)
        for j in range(1, len(unique_ids)):
            axes[y][j].sharey(axes[y][0])
    handles, labels = axes[0][0].get_legend_handles_labels()

    plt.tight_layout(rect=[0.0, 0.0, 1, 0.94])
    axes[0][0].legend(
        handles=handles,
        labels=labels,
        loc="upper center",
        bbox_to_anchor=(len(unique_ids) / 2.0, 1.2),
        ncol=len(join_algos),
        fancybox=True,
        shadow=True,
        fontsize=12,
    )
    plt.savefig("join_benchmark.png")
    plt.show()


if __name__ == "__main__":
    plot_join_benchmark(
        ["runtime", "perf.instructions"],
        ["Runtime (microseconds)", "Executed Instructions (per probe)"],
    )
