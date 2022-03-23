// int weight [12]; //the time of going from S to current
//   int prev [12]; // we got to node a from the value of prev[a]
//   int visited [12] = {0};
//   int lowest = 0;
//   int current = s; // starting position
//   for(int i = 0; i < 12; i++){
//     weight[i] = INT_MAX/2;
//   }
//   weight[s] = 0;

// while(current != e){
// //    printf("%i\n", current);
// //    for(int i = 0; i < 12; i++){
// //      printf("%i ", prev[i]);
// //    }
// //    printf("\n");
// //    for(int i = 0; i < 12; i++){
// //      printf("%i ", visited[i]);
// //    }
// //    printf("\n");
// //    printf("current: %i, e: %i\n", current, e);
//     for(int i = 0; i<12; i++){ //assign new weight of connection to i
//       if(visited[i] == 0 && conn[current][i] != 0 && weight[i]>weight[current]+conn[current][i]){ //new connection is shorter
//         weight[i] = weight[current]+conn[current][i];
//         prev[i] = current;
//       }
//     }
//     //look for the lowest value to be the new current node
//     int min = INT_MAX/2;
//     int index = -1;
//     for (int i = 0; i < 12; i++) {
//       if(visited[i] == 0 && weight[i] < min){
//         index = i;
//         min = weight[i];
//       }
//     }
//     if(index == -1){
//       printf("UNREACHABLE\n");
//       return;
//     }
//     visited[index] = 1;
//     current = index;
//   }

//   printReverse(prev, current, s);
//   printf("%i\n", weight[current]);
  