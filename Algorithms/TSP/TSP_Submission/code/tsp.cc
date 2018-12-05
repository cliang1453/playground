#include <algorithm>
#include <chrono>
#include <cmath>
#include <deque>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <random>
#include <vector>
#include <tuple>

namespace oy
{

template <typename Type = int>
class OyRand
{
public:
    OyRand(Type a = std::numeric_limits<Type>::min(),
           Type b = std::numeric_limits<Type>::max())
        :gen(rd()), dis(a,b){}
    Type get() {return dis(gen); }
    void set_range(Type a, Type b) { dis = std::uniform_int_distribution<Type>(a, b); }
    Type get(Type a, Type b) { return std::uniform_int_distribution<Type>(a, b)(gen); }
    void set_seed(unsigned s){gen.seed(s);}
private:
    std::random_device rd;  // Will be used to obtain a seed for the random number engine
    std::mt19937_64 gen;       // Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<Type> dis;
};

template <> class OyRand<double>
{
    using Type = double;
public:
    OyRand(Type a = std::numeric_limits<Type>::min(),
           Type b = std::numeric_limits<Type>::max())
        :gen(rd()), dis(a,b){}
    Type get() {return dis(gen); }
    void set_range(Type a, Type b) { dis = std::uniform_real_distribution<Type>(a, b); }
    Type get(Type a, Type b) { return std::uniform_real_distribution<Type>(a, b)(gen); }
    void set_seed(unsigned s){gen.seed(s);}
private:
    std::random_device rd;  // Will be used to obtain a seed for the random number engine
    std::mt19937_64 gen;       // Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<Type> dis;
};

}

namespace oy_sa_tsp
{

struct Point{
    double x,y;
};

struct SaConfig {
    std::string name;
    double init_temperature;
    double temp_decay_factor;
    unsigned num_neighbor_explore;
    unsigned num_neighbor_used;
    unsigned max_iter;
    unsigned cut_off_time_in_seconds;
    unsigned seed;
};

struct History{
    long long time;
    double temperature;
    std::vector<unsigned> sol;
    unsigned dis;
    bool operator<(const History& b)const{return dis<b.dis;}
};

class SA_TSP {
public:
    auto get_d(unsigned i, unsigned j){return d[i][j];}
    void set_geo(bool b){geo=b;}
    void set_restart(bool b){restart=b;}
    void add_point(double x, double y) { points.push_back(Point{x,y}); }
    ~SA_TSP(){}
    auto generate_neighbors(unsigned num_neighbor_explore, const std::vector<unsigned>& ccur, unsigned seed=0) {
        auto sz = ccur.size();
        static oy::OyRand<unsigned> rint;
        static oy::OyRand<double> rreal;
        static bool first_time=true;
        if(first_time){
            first_time=false;
            if(seed)
            {
                rint.set_seed(seed);
                rreal.set_seed(seed);
            }
        }
        std::vector<std::tuple<unsigned, unsigned, unsigned> > veee;
        for (auto i=0u; i<num_neighbor_explore; i++)
            veee.push_back({rint.get(0, sz-1), rint.get(0, sz-2), 0});

        std::vector<unsigned>& cur = const_cast<std::vector<unsigned>& >(ccur);
        for (auto &p : veee)
        {
            // auto& [v1, v2, dis] = p;
            auto & v1 = std::get<0>(p);
            auto & v2 = std::get<1>(p);
            auto & dis = std::get<2>(p);
            v2 = (v1 + v2)%sz;
            std::swap(cur[v1],cur[v2]);
            dis = distance_of_cycle(cur);
            std::swap(cur[v1],cur[v2]);
        }
        sort(veee.begin(), veee.end(), [](auto&a, auto& b){return std::get<2>(a) < std::get<2>(b);});
        return veee;
    }
    auto simulated_annealing(SaConfig config, const std::vector<unsigned> & best = {})
    {
        // config part;
        double temperature = config.init_temperature;
        const double temp_decay_factor = config.temp_decay_factor;
        const unsigned num_neighbor_explore = config.num_neighbor_explore;
        const unsigned num_neighbor_used = config.num_neighbor_used;
        const unsigned max_iter = config.max_iter;

        prepare();
        auto cur = initial_guess(config.seed);
        auto curdis = distance_of_cycle(cur);
        auto best_dis = curdis;
        auto best_sol = cur;
        std::vector<std::pair<std::chrono::milliseconds, unsigned> > trace;
        std::cout << "offset: " << d_offset << '\n';
        if (best.size())
            std::cout << "best: " << distance_of_cycle(best) + d_offset << '\n';
        oy::OyRand<unsigned> rint;
        oy::OyRand<double> rreal;

        if (config.seed != 0)
        {
            rint.set_seed(config.seed);
            rreal.set_seed(config.seed);
        }

        std::vector<History> history;
        auto last_restart = 0u;
        auto restart_count = 1u;
        auto not_restart_count = 1u;
        auto time_start = std::chrono::steady_clock().now();
        for (auto iter = 0u, accepted_count = 0u; iter<max_iter; iter++)
        {
            if (iter % 1000 == 0)
            {
                std::cout << "iter " << iter << ": " << curdis + d_offset << '\t' << "acc_count:" << accepted_count << '\n';
                accepted_count = 0u;
                auto t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock().now()-time_start);

                if (t.count() > config.cut_off_time_in_seconds*1000)
                {
                    break;
                }
                history.push_back({t.count(),temperature,cur,curdis+d_offset});
                unsigned int n = 10;
                unsigned int nn = restart_count*n;
                if (restart && iter > last_restart + n*1000 && history.size()>nn && (history.end()-n)->dis == std::min_element(history.end()-n, history.end())->dis)
                {
                    auto me = std::min_element(history.end()-n, history.end())->dis;
                    auto Me = std::max_element(history.end()-n, history.end())->dis;
                    if (me==Me)
                    {
                        std::cout << "restart all \n";
                        cur = initial_guess(config.seed*iter);
                        curdis = distance_of_cycle(cur);
                        history.clear();
                        restart_count = 1;
                        not_restart_count = 1;
                        temperature = config.init_temperature;
                    }
                    else
                    {
                        std::cout << "restart \n";
                        auto h = *(history.rbegin()+nn);
                        temperature = h.temperature;
                        cur = h.sol;
                        curdis = h.dis;
                        history.erase(history.end()-n, history.end());
                        last_restart = iter;
                        restart_count++;
                        not_restart_count = 0;
                    }
                }
                else
                {
                    not_restart_count++;
                    if (not_restart_count > 40) restart_count = 2;
                }
            }
            temperature *= temp_decay_factor; // cool down;
            auto veee = generate_neighbors(num_neighbor_explore, cur, config.seed);
            //auto [v1, v2, dis] = veee.at(rint.get(0,num_neighbor_used-1));
            auto v1 = std::get<0>(veee.at(rint.get(0,num_neighbor_used-1)));
            auto v2 = std::get<1>(veee.at(rint.get(0,num_neighbor_used-1)));
            auto dis = std::get<2>(veee.at(rint.get(0,num_neighbor_used-1)));
            if (dis < curdis || std::exp(-(static_cast<int>(dis-curdis))/temperature) > rreal.get(0,1))
            {
                curdis = dis;
                std::swap(cur[v1],cur[v2]);
                if (curdis < best_dis) {
                    best_dis = curdis;
                    best_sol = cur;
                    trace.emplace_back(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock().now()-time_start), curdis + d_offset);
                }
                accepted_count++;
            }
        }
        return std::make_tuple(best_sol, best_dis + d_offset, trace);
    }
    int run(SaConfig config, std::string sol_file, std::string trace_file ) {
        if (sol_file.empty() || trace_file.empty())
        {
            simulated_annealing(config, {});
            return 0;
        }
        std::ofstream fsol(sol_file);
        if (fsol.bad()) return -1;
        std::ofstream ftrace(trace_file);
        if (ftrace.bad()) return -1;
        //auto [sol, soldis, trace] = simulated_annealing(config, {});
        auto sol = std::get<0>(simulated_annealing(config, {}));
        auto soldis = std::get<1>(simulated_annealing(config, {}));
        auto trace = std::get<2>(simulated_annealing(config, {}));
        fsol << soldis << '\n';
        for (auto i=0u; i<sol.size(); i++) {
            auto a = sol[i];
            auto b = sol[(i+1)%sol.size()];
            fsol << a << ' ' << b << ' ' << distance_between(a,b) << '\n';
        }
        for(const auto &i:trace)
            ftrace<<i.first.count()/1000.0 << ' ' << i.second << '\n';
        return 0;
    }

private:
    std::vector<unsigned> initial_guess(unsigned seed=0) const
    {
        if(seed==0)
        {
            std::vector<unsigned> v(points.size());
            std::iota(v.begin(), v.end(), 0);
            std::random_shuffle(v.begin(), v.end());
            return v;
        }
        else
        {
            std::mt19937_64 gen(seed);
            std::vector<unsigned> v(points.size());
            std::iota(v.begin(), v.end(), 0);
            std::random_shuffle(v.begin(), v.end(), [&gen](int i){return std::uniform_int_distribution<int>(0,i)(gen);});
            return v;
        }

    }
    unsigned distance_of_cycle(const std::vector<unsigned>& cycle) const
    {
        auto dis = 0u;
        auto prev = cycle.back();
        for (auto i=0u; i<cycle.size(); prev = cycle[i++])
            dis += d.at(prev).at(cycle[i]);
        return dis;
    }
    unsigned distance_between(unsigned int i, unsigned int j)
    {
        return (geo?d_geo(i,j):d_euc2d(i,j));
    }

    template <typename T>
    auto transpose(const std::vector<std::vector<T> >& out) {
        std::vector<std::vector<T> > outtrans(out[0].size(), std::vector<T>(out.size()));
        for (size_t i = 0; i < out.size(); ++i)
            for (size_t j = 0; j < out[0].size(); ++j)
                outtrans[j][i] = out[i][j];
        return outtrans;
    }
    void prepare()
    {
        auto sz = points.size();
        d = std::vector<std::vector<unsigned> >(sz, std::vector<unsigned>(sz,0));
        for (auto i=0u; i<points.size(); i++)
            for (auto j=0u; j<points.size(); j++)
                if (i!=j)
                    d[i][j] = distance_between(i,j);
                else
                    d[i][j] = std::numeric_limits<unsigned>::max();
        d_offset = 0;
        auto subtract_common = [this]{
            for (auto i=0u; i<points.size(); i++)
            {
                auto mind = *std::min_element(d[i].begin(), d[i].end());
                d_offset += mind;
                std::transform(d[i].begin(), d[i].end(), d[i].begin(), [mind](auto y){return y-mind;});
            }
        };
        subtract_common();
        d = transpose(d);
        subtract_common();
        d = transpose(d);
    }
    unsigned d_euc2d(unsigned i, unsigned j){
        auto dx = points.at(i).x - points.at(j).x;
        auto dy = points.at(i).y - points.at(j).y;
        return std::round(std::sqrt(dx*dx+dy*dy));
    }
    unsigned d_geo(unsigned i, unsigned j){
        static constexpr auto PI = 3.141592;
        auto f = [](Point p)->Point{
            auto degix = static_cast<int>( p.x );
            auto minix = p.x - degix;
            auto degiy = static_cast<int>( p.y );
            auto miniy = p.y - degiy;
            auto latitude_i = PI * (degix + 5.0 * minix / 3.0 ) / 180.0;
            auto longitude_i = PI * (degiy + 5.0 * miniy / 3.0 ) / 180.0;
            return {latitude_i, longitude_i};
        };
        auto pi = f(points.at(i));
        auto pj = f(points.at(j));
        static constexpr auto RRR = 6378.388;
        auto q1 = std::cos( pi.y - pj.y );
        auto q2 = std::cos( pi.x - pj.x );
        auto q3 = std::cos( pi.x + pj.x );
        auto dij = static_cast<unsigned int>( RRR * acos( 0.5*((1.0+q1)*q2 - (1.0-q1)*q3) ) + 1.0);
        return dij;
    }
    std::vector<Point> points;
    std::vector<std::vector<unsigned> > d;
    unsigned d_offset;
    bool geo = false;
    bool restart = true;
};

}

/*
int main()
{
    // oy_sa_tsp::SaConfig config{10000, 0.9999, 100, 10};
    oy_sa_tsp::SaConfig config{"", 10000, 0.9999, 500, 10, 100000};
    //double T;double decay;unsigned int c,d,e;
    //std::cin>>T>>decay>>c>>d>>e;
    //oy_sa_tsp::SaConfig config{T,decay,c,d};
    oy_sa_tsp::SA_TSP tsp;
#include "berlin.inc"
    for (auto& b:best) b--;
    tsp.simulated_annealing(config, best);
    return 0;
}
*/

int tmain(int argc, char *argv[])
{
    unsigned int cut_off_time_in_seconds = std::stoul(argv[4]);
    unsigned int seed = std::stoul(argv[5]);
    oy_sa_tsp::SaConfig config{"", 10000, 0.9999, 100, 10, 500000, cut_off_time_in_seconds, seed};
    oy_sa_tsp::SA_TSP tsp;
    tsp.set_restart(true);
    std::string name = argv[1];
    std::ifstream fin(name);
    std::string s;
    while(fin>>s) {
        if (s=="GEO")
            tsp.set_geo(true);
        if (s=="NODE_COORD_SECTION") {
            int a;double b,c;
            while(fin>>a>>b>>c)
                tsp.add_point(b, c);
            break;
        }
    }
    tsp.run(config, argv[2], argv[3]);
    return 0;
}

int main(int argc, char* argv[])
{
    return tmain(argc, argv);
}

extern "C" {

int f(char *argv1, char *argv2, char *argv3, char *argv4, char *argv5)
{
    char* argv[7] = { nullptr, argv1,argv2,argv3,argv4,argv5,nullptr };
    return tmain(2,argv);
}

}
