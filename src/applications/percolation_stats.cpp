#include "applications/percolation_stats.h"

applications::percolation_stats::percolation_stats(const uint n, const uint trials)
    : m_counts(trials, 0), m_trials(trials), m_size(n)
{
    run();
}

// perform trials independent experiments on an n-by-n grid
void applications::percolation_stats::run()
{
    uint consumed_trials = 0;

    while (m_trials != consumed_trials)
    {
        uint counter = 0;

        percolation percolation(m_size);

        while (!percolation.percolates())
        {
            uint row = std::experimental::randint(uint(0), m_size - 1);
            uint col = std::experimental::randint(uint(0), m_size - 1);

            if (!percolation.is_open(row, col))
            {
                percolation.open(row, col);
                ++counter;
            }
        }

        m_counts[consumed_trials] = double(counter) / (m_size * m_size);
        ++consumed_trials;
    }
}

// sample mean of percolation threshold
double applications::percolation_stats::mean() const
{
    double sum = 0;
    for (uint i = 0; i < m_counts.size(); ++ i)
        sum += m_counts[i];

    return double(sum) / m_counts.size();
}

// sample standard deviation of percolation threshold
double applications::percolation_stats::standard_deviation() const
{
    if (m_counts.size() < 2)
        return 0;

    double average = mean();
    double sum_sq_diff = 0.0;

    for (uint i = 0; i < m_counts.size(); ++i)
    {
        double diff = m_counts[i] - average;
        sum_sq_diff += diff * diff;
    }

    return sqrt(sum_sq_diff / (m_counts.size() - 1));
}

// low  endpoint of 95% confidence interval
double applications::percolation_stats::confidence_lo() const
{
    return mean() - 1.96 * standard_deviation() / sqrt(m_trials);
}

// high endpoint of 95% confidence interval
double applications::percolation_stats::confidence_hi() const
{
    return mean() + 1.96 * standard_deviation() / sqrt(m_trials);
}
