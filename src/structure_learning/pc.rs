use std::collections::{HashSet, HashMap};

use crate::{graph::Graph, dataframe::DataFrame, iter::combinations};

type SeperationSets = HashMap<(usize, usize), HashSet<usize>>;

fn orient_edges(graph: &mut Graph, sep_sets: &SeperationSets) {

}

fn pc(data: DataFrame) -> Graph {
    let mut graph = Graph::fully_connected(data.names().clone());

    let mut indices: HashSet<usize> = (0..graph.len()).collect(); 
    
    let mut sep_sets: SeperationSets = HashMap::new();

    for n in 0..=(graph.len() - 2) {
        for x in 0..graph.len() {
            indices.remove(&x);

            for y in (x + 1)..graph.len() {
                indices.remove(&y);

                for zs in combinations(n, indices.iter().collect()) {
                    let zs = zs.into_iter().copied().collect();

                    if data.conditionally_independent(x, y, &zs) {
                        graph.remove_edge(x, y);
                        graph.remove_edge(y, x);
                        sep_sets.insert((x, y), zs.into_iter().collect());
                    }
                }

                indices.insert(y);
            }

            indices.insert(x);
        }
    }

    orient_edges(&mut graph, &sep_sets);

    return graph;
}


fn shortcut_pc(data: DataFrame) -> Graph {
    fn sbc(graph: &Graph, x: usize, y: usize) -> (HashSet<usize>, HashSet<usize>, HashSet<usize>) {
        let (mut s, mut b, mut c) = {
            let mut b = (0..graph.len()).collect::<HashSet<_>>();
            b.remove(&x);
            b.remove(&y);
            (HashSet::new(), b.clone(), b)
        };

        let mut graph_prime = graph.clone();

        for z in graph.neighbors(x).intersection(graph.neighbors(y)) {
            let r = graph.component(*z);
            c.retain(|x| !r.contains(&x));

            s = s.union(&r.intersection(&graph.neighbors(x).iter().copied().collect()).copied().collect()).copied().collect();
            for x in r {
                graph_prime.isolate(x);
            }
        }

        for &z in graph_prime.neighbors(x) {
            for &v in graph_prime.neighbors(z) {
                if v == z { continue; }

                let r_v = graph_prime.component(v);

                if r_v.into_iter().any(|w| graph_prime.has_edge(w, z)) {
                    b.insert(z);
                    b.insert(v);
                }
            }
        }

        (s, b, c)
    }

    let mut graph = Graph::fully_connected(data.names().clone());
    
    let mut indices: HashSet<usize> = (0..graph.len()).collect(); 
    
    let mut sep_sets: SeperationSets = HashMap::new();

    
    for m in 0..=graph.len() - 2 {
    // for m in (0..=graph.len() - 2).map(
    //     |x| if x & 1 == 0 { graph.len() - 2 - x >> 1 } else { x >> 1 }
    // ).collect::<Vec<_>>() {
    //     dbg!(graph.max_degree());
    //     dbg!(graph.min_degree());

        // if m + 1 > graph.max_degree() || m + 1 < graph.min_degree() { continue; }

        for x in 0..graph.len() {
            for y in graph.neighbors(x).clone() {
                let (s, b, _c) = sbc(&graph, x, y);

                for u in combinations(m, s.iter().collect()) {
                    let u = u.into_iter().copied().collect();
                    
                    let sep_set = b.union(&u).copied().collect();

                    if data.conditionally_independent(x, y, &sep_set) {
                        graph.remove_edge(x, y);
                        sep_sets.insert((x, y), sep_set.into_iter().collect());
                    }
                }
            }

            indices.insert(x);
        }
    }

    orient_edges(&mut graph, &sep_sets);

    return graph;
}

mod tests {
    use crate::dataframe::DataFrame;

    #[test]
    fn bench() {
        use super::*;
        use std::time::Instant;

        let mut data = DataFrame::new(["A", "B", "C", "D", "E", "F", "G"].iter().map(|x| x.to_string()).collect());
        
        for _ in 0..1000 {
            data.add_row(vec![0, 0, 0, 0, 0, 0, 0]);
        }

        let data_1 = data.clone();
        let data_2 = data;

        let start_sc = Instant::now();
        let result_2 = shortcut_pc(data_2);
        let sc_time = start_sc.elapsed();

        let start_pc = Instant::now();
        let result_1 = pc(data_1);
        let pc_time = start_pc.elapsed();


        println!("pc: {} ms\nsc: {} ms", pc_time.as_millis(), sc_time.as_millis());

        println!("{:?}", result_1);
        println!("{:?}", result_2);

    }
}