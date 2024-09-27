### Project
`build.sh`: build `main.cpp` to `build/learning_taskflow`

`run.sh`: run `build/learning_taskflow` and generate Taskflow file(.dot) to directory `task_graph`

`visualize.sh`: convert `task_graph/*.dot` to `task_graph/*.svg` by [graphviz](https://www.graphviz.org/download/)

### install `graphviz` and convert '.dot' to '.svg'
```shell
# mac install
brew install graphviz
dto -Tsvg example.dot -o example.svg
```

