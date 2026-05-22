def merge(intervals):
    if not intervals:
        return []

    # Sort intervals based on the starting point
    intervals.sort(key=lambda x: x[0])

    # Initialize the merged list with the first interval
    merged = [intervals[0]]

    for current in intervals[1:]:
        last_merged = merged[-1]

        # Check if there is an overlap
        if current[0] <= last_merged[1]:  # Overlapping intervals
            # Merge the current interval with the last one in merged list
            last_merged[1] = max(last_merged[1], current[1])
        else:
            # No overlap, add the current interval to the merged list
            merged.append(current)

    return merged

# Example usage:
intervals = [[1, 3], [2, 6], [8, 10], [15, 18]]
print(merge(intervals))  # Output: [[1, 6], [8, 10], [15, 18]]
