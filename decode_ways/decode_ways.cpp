///
// Problem Source: https://leetcode.com/problems/decode-ways/
// Dynamic Programming solution.
///
class Solution
{
    public:
    vector<bool> isCharacterValid;
    vector<int> numberPossibleEncodings;
    
    int numDecodings(string s)
    {
        isCharacterValid = vector<bool>(s.length());
        numberPossibleEncodings = vector<int>(s.length());
        
        numDecodingsRec(s);
        
        return (s.length() > 0 && isCharacterValid[0]) ? numberPossibleEncodings[0] : 0;
    }
    
    void numDecodingsRec(string s)
    {
        int positionInDA = isCharacterValid.size() - s.length();
        
        if(s.length() == 0)
        {
            return;
        }
        else if (s.length() == 1)
        {
            if(s[0] > '0')
            {
                isCharacterValid[positionInDA] = true;
                numberPossibleEncodings[positionInDA] = 1;
            }
            else
            {
                isCharacterValid[positionInDA] = false;
                numberPossibleEncodings[positionInDA] = 0;
            }
            
            return;
        }
        
        numDecodingsRec(s.substr(1, s.length() - 1));
        
        isCharacterValid[positionInDA] = true;
        bool isValidDouble = false;
        switch(s[0])
        {
            case '0':
                isCharacterValid[isCharacterValid.size() - s.length()] = false;
                break;
            case '1':
                isValidDouble = (s.length() > 1 && (s.length() < 3 || isCharacterValid[positionInDA + 2]));
                break;
            case '2':
                isValidDouble = ((s.length() > 1 && s[1] < '7') &&
                  (s.length() < 3 || isCharacterValid[positionInDA + 2]));
                break;
            default:
                break;
        }
        
        if(isValidDouble)
        {
            int fromSingle = isCharacterValid[positionInDA + 1] ? numberPossibleEncodings[positionInDA + 1] : 0;
            int fromDouble = (positionInDA + 2 < numberPossibleEncodings.size()) ? numberPossibleEncodings[positionInDA + 2] : 1;
            numberPossibleEncodings[positionInDA] = fromSingle + fromDouble;
        }
        else
        {
            numberPossibleEncodings[positionInDA] = isCharacterValid[positionInDA + 1] ? numberPossibleEncodings[positionInDA + 1] : 0;
        }
    }
};
