Card = (name) ->
  @name = name
  @group = Math.floor(Math.random()*4)
  @size = Math.floor(Math.random()*3)
  null

viewModel = {
  test: ko.observable("test value")
  cards: ko.observableArray(new Card(name) for name in ["1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11"])

  modes: ["Physical machines", "Virtual machines", "Storage"]
  selectedMode: ko.observable("Physical machines")
  selectMode: (mode) -> this.selectedMode(mode)
}

window.viewModel = viewModel

ko.applyBindings viewModel


$('#cards').isotope {
    itemSelector : '.card'
    layoutMode : 'fitRows'
    getSortData: {
      group: (elem) -> elem.attr("data-group")
      size: (elem) -> elem.attr("data-size")
    }
}

$("#sortByGroup").click -> $("#cards").isotope {sortBy: "group"}
$("#sortBySize").click -> $("#cards").isotope {sortBy: "size"}
