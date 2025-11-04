def findShortestSubArray(nums):
    if not nums:
        return 0

    count = {}
    first_seen = {}
    last_seen = {}

    # Populate dictionaries
    for i, num in enumerate(nums):
        if num not in first_seen:
            first_seen[num] = i
        last_seen[num] = i
        count[num] = count.get(num, 0) + 1

    # Determine the degree of the array
    degree = max(count.values())

    # Initialize minimum length to a large number
    min_length = float('inf')

    # Find the smallest subarray with the same degree
    for num in count:
        if count[num] == degree:
            min_length = min(min_length, last_seen[num] - first_seen[num] + 1)

    return min_length
