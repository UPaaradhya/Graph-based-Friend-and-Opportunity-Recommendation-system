// Recommendation data structures and opportunity-tree node.
// ================= RECOMMENDATION STRUCTURES =================

// Friend recommendation

struct Recommendation {

    int userIndex;
    int score;

    bool operator<(const Recommendation& other) const {

        return score < other.score;
    }
};


// Opportunity recommendation

struct OpportunityRecommendation {

    int opportunityIndex;
    int score;

    bool operator<(const OpportunityRecommendation& other) const {

        return score < other.score;
    }
};


// ================= TREE NODE =================

class OpportunityNode {
public:

    string category;

    vector<OpportunityNode*> children;

    OpportunityNode(string name) {

        category = name;
    }
};


