def count_islands(grid):
    # dfs here
    def dfs(grid, i, j):
        if i < 0 or i >= len(grid) or j < 0 or j >= len(grid[i]) or grid[i][j] == '0':
            return

        grid[i][j] = '0'
        dfs(grid, i-1, j)
        dfs(grid, i+1, j)
        dfs(grid, i, j-1)
        dfs(grid, i, j+1)

    # two for loops
    islands = 0
    for i in range(len(grid)):
        for j in range(len(grid[i])):
            if grid[i][j] == '1':
                dfs(grid, i, j)
                islands = islands + 1

    return islands


if __name__ == "__main__":
    # output 1
    grid1 = [
      ["1","1","1","1","0"],
      ["1","1","0","1","0"],
      ["1","1","0","0","0"],
      ["0","0","0","0","0"]
    ]
    
    # output 3
    grid2 = [
      ["1","1","0","0","0"],
      ["1","1","0","0","0"],
      ["0","0","1","0","0"],
      ["0","0","0","1","1"]
    ]

    print(count_islands(grid1))
    print(count_islands(grid2))
