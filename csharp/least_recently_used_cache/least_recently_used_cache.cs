public class LRUCache
{
    private readonly int maxCapacity;
    
    private readonly Dictionary<int, LinkedListNode<KeyValuePair<int,int>>> keyValueMapping = new();
    private readonly LinkedList<KeyValuePair<int,int>> leastRecentlyUsedKeys = new();
    
    // Some mechanism to keep track of access order.
    public LRUCache(int capacity)
    {
        maxCapacity = capacity;
    }
    
    // Target O(1) average time complexity
    public int Get(int key)
    {
        if(!keyValueMapping.TryGetValue(key, out LinkedListNode<KeyValuePair<int,int>> found))
        {
            return -1;
        }
        
        // Move found element to new Position in LRU
        leastRecentlyUsedKeys.Remove(found);
        leastRecentlyUsedKeys.AddLast(found);
        
        return found.Value.Value;
    }
    
    // Target O(1) average time complexity
    public void Put(int key, int value)
    {
        
        // Can't store anything in a LRU with capacity of 0.
        if(maxCapacity == 0)
        {
            return;
        }
        
        var freshNode = leastRecentlyUsedKeys.AddLast(new KeyValuePair<int,int>(key, value));
        if(keyValueMapping.TryGetValue(key, out LinkedListNode<KeyValuePair<int,int>> found))
        {
            leastRecentlyUsedKeys.Remove(found);
            keyValueMapping[key] = freshNode;            
        }
        else
        {
            keyValueMapping.Add(key, freshNode);
            
            // Remove Node if we exceeded capacity.
            if(keyValueMapping.Count > maxCapacity)
            {
                var leastRecentlyUsedKey = leastRecentlyUsedKeys.First();
                keyValueMapping.Remove(leastRecentlyUsedKey.Key);
                leastRecentlyUsedKeys.Remove(leastRecentlyUsedKey);
            }
        }
    }
}
