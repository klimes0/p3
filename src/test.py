def odd_squared(nums):
    odd_sum = 0;
    for i in range(len(nums)):
        if i%2 == 1:
            odd_sum = nums[i] + odd_sum
    odd_sum = odd_sum**2
    return odd_sum
nums = [5,6,7,8]
print(odd_squared(nums))